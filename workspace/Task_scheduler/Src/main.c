#include "main.h"

uint8_t current_task = 1;
uint32_t g_ticks_count = 0;

typedef struct {
	uint32_t psp_value;
	uint32_t block_count;  // ticks count
	uint8_t current_state; // running state
	void (*task_handler)(void);
}TCB_t; // Task Control Block

TCB_t user_tasks[MAX_TASKS];

int main(void)
{
	enable_processor_fault(); // sometimes we may overwrite SFs or may breach stack boundaries (so called illegal) so tracking such faults is important.

	init_scheduler_stack(SCHED_STACK_START); // initializing the location for MSP to point to

	init_task_stack();	// initializing the location for PSP to point to and initializing dummy stack frame (SF1 + SF2)

	led_init_all();

	init_systick_timer(TICK_HZ); // initializing the SysTick timer for scheduling

	switching_sp_to_psp(); // done before launching the task; can also be done by using SVC instruction

	task1_handler(); // launching the TASK 1

	for(;;);
}

void init_systick_timer(uint32_t tick_hz) {
	uint32_t *pSYST_RVR = (uint32_t *)0xE000E014;
	uint32_t *pSYST_CSR = (uint32_t *)0xE000E010;

	uint32_t count_val = (SYSTICK_TIMER_CLOCK/tick_hz)-1; // reload val is suggested to enter N-1
	*pSYST_RVR &= ~(0x00FFFFFF);
	*pSYST_RVR |= count_val;
	/* Configuring controls for SysTick timer. */
	*pSYST_CSR |= (1 << 1);	// Enables systick exception request
	*pSYST_CSR |= (1 << 2);	// Set internal clock source
	*pSYST_CSR |= (1 << 0);	// Enable systick
//	printf("Exiting...\n");
}

__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack) {
	/* R0 for arg 1 */
	__asm__ volatile("MSR MSP, %0":: "r"(sched_top_of_stack) : );
	__asm__ volatile("BX LR"); // Return from the current function to the address stored in LR (return;).
}

void init_task_stack(void)
{
	user_tasks[0].current_state = TASK_READY_STATE;	// Always running state for the idle task
	user_tasks[1].current_state = TASK_READY_STATE;
	user_tasks[2].current_state = TASK_READY_STATE;
	user_tasks[3].current_state = TASK_READY_STATE;
	user_tasks[4].current_state = TASK_READY_STATE;

	user_tasks[0].psp_value = IDLE_STACK_START;
	user_tasks[1].psp_value = T1_STACK_START;
	user_tasks[2].psp_value = T2_STACK_START;
	user_tasks[3].psp_value = T3_STACK_START;
	user_tasks[4].psp_value = T4_STACK_START;

	user_tasks[0].task_handler = idle_task;
	user_tasks[1].task_handler = task1_handler;
	user_tasks[2].task_handler = task2_handler;
	user_tasks[3].task_handler = task3_handler;
	user_tasks[4].task_handler = task4_handler;

	uint32_t *pPSP;

	for(int i=0; i<MAX_TASKS; i++){
		pPSP = (uint32_t *) user_tasks[i].psp_value;	// *psp pointing to task stack start
		pPSP--;
		*pPSP = DUMMY_xPSR;	// EPSR
		pPSP--;
		*pPSP = (uint32_t) user_tasks[i].task_handler; // PC
		pPSP--;
		*pPSP = EXC_RETURN;	//	LR
		for(int j=0; j<13; j++) {	//	R0 to R12
			pPSP--;
			*pPSP = 0;
		}
		user_tasks[i].psp_value = (uint32_t)pPSP; // preserving the PSP value for every tasks
	}
}

uint32_t get_psp_val(void) {
	return user_tasks[current_task].psp_value;
}

void save_psp_value(uint32_t current_psp_val) {
	user_tasks[current_task].psp_value = current_psp_val;
}

void update_next_task(void)
{
	int state = TASK_BLOCKED_STATE;
	/* To schedule the tasks in Round Robin Fashion. */
	for(int i=0; i<MAX_TASKS; i++) {
		current_task++;		// updating to the next task
		current_task %= MAX_TASKS;
		state = user_tasks[current_task].current_state;
		if((state == TASK_READY_STATE) && (current_task != 0)) {
			break;
		}
	}
	if(state != TASK_READY_STATE) {
		current_task = 0;
	}
}

void update_g_ticks_count(void) {
	g_ticks_count++;
}

void unblock_tasks(void) {
	for(int i = 1; i < MAX_TASKS; i++) {
		if(user_tasks[i].current_state != TASK_READY_STATE) {
			if(user_tasks[i].block_count == g_ticks_count) {
				user_tasks[i].current_state = TASK_READY_STATE;
			}
		}
	}
}

void SysTick_Handler(void) /* Unblocking tasks */
{
	uint32_t *pICSR = (uint32_t *)0xE000ED04;

	update_g_ticks_count();

	unblock_tasks(); // Unblock the task if the delay period is elapsed

	//Pend the PendSV exception
	*pICSR |= (1 << 28); // Schedules next task
}

__attribute__((naked)) void PendSV_Handler(void) { /* Context switching */
	/* Save the context of the current task */
	// 1. Get the PSP value of current task
	__asm__ volatile("MRS R0, PSP");
	// 2. Store SF2 using that PSP value (R4 to R11)
	__asm__ volatile("STMDB R0!, {R4 - R11}");	//	Store multiple registers, decrement byte, '!' used to update R0 to the latest stored register address.
	// 3. preserve current PSP value
	__asm__ volatile("PUSH {LR}");
	__asm__ volatile("BL save_psp_value");

	/* Retrieve the context of the next task */
	// 1. Decide next task to run
	__asm__ volatile("BL update_next_task");
	// 2. Get its past PSP value
	__asm__ volatile ("BL get_psp_val");
	// 3. Retrieve SF2 using that PSP value (R4 to R11)
	__asm__ volatile("LDMIA R0!, {R4 - R11}");	//	Load multiple registers, increment address, '!' used to update R0 to the latest stored register address.
	// 4. Update PSP and exit
	__asm__ volatile("MSR PSP, R0");

	__asm__ volatile("POP {LR}");
// can be used instead of push and pop LR operations.
//	__asm__ volatile("MOV LR, %0": :"r"(0xFFFFFFFD)); // MSP has the unchanged scheduler stack address when this line is uncommented

	__asm__ volatile("BX LR"); // this by default sets LR to 0xFFFFFFFD which is -3 instead of 0xFFFFFFF9 (-7) this is becuz the CPU sets LR depending on what stack pointer was in use when SysTick fired.
}

void schedule(void)
{
	//pend the pendsv exception
	uint32_t *pICSR = (uint32_t*)0xE000ED04;
	*pICSR |= ( 1 << 28);
}

void task_delay(uint32_t ticks_count)
{
	INTERRUPT_DISABLE(); // k_spin_lock(); for atomic operation
	if(current_task != 0)
	{
		user_tasks[current_task].block_count = g_ticks_count + ticks_count;
		user_tasks[current_task].current_state = TASK_BLOCKED_STATE;
		schedule();
	}
	INTERRUPT_ENABLE();	// k_spin_unlock();
}

void __attribute__((naked)) switching_sp_to_psp(void) {
	/* initialize the PSP to task 1's private stack start address */
	__asm__ volatile ("PUSH {LR}");	// preserve LR (connects back to main()) before changing it
	__asm__ volatile ("BL get_psp_val"); // branch to get_psp_val and come back (link) to this func itself (this func addr is in LR)
	__asm__ volatile ("MSR PSP, R0"); // the return psp_of_tasks is stored in R0 so PSP = R0
	__asm__ volatile ("POP {LR}");	// pops back the new LR value so now it only connects to main()

	/* Change SP to PSP using CONTROL register */
		//__asm__ volatile ("MRS R0, CONTROL\n\t" "ORR R0, R0, #0x1\n\t" "MSR CONTROL, R0\n\t");
	__asm__ volatile ("MOV R0, #0x02");
	__asm__ volatile ("MSR CONTROL, R0");

	__asm__ volatile ("BX LR"); // return; (branch to main())
}

void enable_processor_fault(void) {
	uint32_t *pSHCRS = (uint32_t *)0xE000ED24;
	*pSHCRS |= SHCRS_USAGEFAULT | SHCRS_BUSFAULT | SHCRS_MEMFAULT;
}
