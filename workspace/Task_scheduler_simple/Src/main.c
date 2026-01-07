#include "main.h"

uint32_t psp_of_tasks[MAX_TASKS] = {T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START}; // task SPs
uint32_t task_handlers[MAX_TASKS];
uint8_t current_task = 0;

int main(void)
{
	enable_processor_fault(); // sometimes we may overwrite SFs or may breach stack boundaries (so called illegal) so tracking such faults is important.

	init_scheduler_stack(SCHED_STACK_START); // initializing the location for MSP to point to

	task_handlers[0] = (uint32_t)task1_handler;
	task_handlers[1] = (uint32_t)task2_handler;
	task_handlers[2] = (uint32_t)task3_handler;
	task_handlers[3] = (uint32_t)task4_handler;

	init_task_stack();	// initializing the location for PSP to point to and initializing dummy stack frame (SF1 + SF2)

	led_init_all();

	init_systick_timer(TICK_HZ); // initializing the SysTick timer for scheduling

	switching_sp_to_psp(); // done before launching the task; can also be done by using SVC instruction

	task1_handler(); // launching the TASK 1

    /* Loop forever */
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

void init_task_stack(void) {
	uint32_t *pPSP;
	for(int i=0; i<MAX_TASKS; i++){
		pPSP = (uint32_t *)psp_of_tasks[i];
		pPSP--;
		*pPSP = DUMMY_xPSR;	// EPSR
		pPSP--;
		*pPSP = task_handlers[i]; // PC
		pPSP--;
		*pPSP = EXC_RETURN;	//	LR
		for(int j=0; j<13; j++) {	//	R0 to R12
			pPSP--;
			*pPSP = 0;
		}
		psp_of_tasks[i] = (uint32_t)pPSP; // preserving the PSP value for every tasks
	}
}

uint32_t get_psp_val(void) {
	return psp_of_tasks[current_task];
}

void save_psp_value(uint32_t current_psp_val) {
	psp_of_tasks[current_task] = current_psp_val;
}

void update_next_task(void) {
	current_task++;
	current_task %= MAX_TASKS;
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

__attribute__((naked)) void SysTick_Handler(void) {
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

//	__asm__ volatile("MOV LR, %0": :"r"(0xFFFFFFFD)); // MSP has the unchanged scheduler stack address when this line is uncommented

	__asm__ volatile("BX LR"); // this by default sets LR to 0xFFFFFFFD which is -3 instead of 0xFFFFFFF9 (-7)	\
					this is becuz the CPU sets LR depending on what stack pointer was in use when SysTick fired.
}

void enable_processor_fault(void) {
	uint32_t *pSHCRS = (uint32_t *)0xE000ED24;
	*pSHCRS |= SHCRS_USAGEFAULT | SHCRS_BUSFAULT | SHCRS_MEMFAULT;
}

