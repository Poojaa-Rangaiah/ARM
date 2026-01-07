#include <stdint.h>
#include <stdio.h>

void HardFault_Handler() {			// executed when SFR is attempted in NPAL
	printf("Hard fault detected.\n");
	while(1);
}

// Access level demonstration of the ARM processor
void change_access_level_unpriv() {		// changing to NPAL
	__asm volatile (
			"MRS R0, CONTROL\t\n"		//	Read
			"ORR R0, R0, #0x01\t\n"		//	Modify
			"MSR CONTROL, R0\t\n"		//	write
	);
}

void generate_interrupt() {
	uint32_t *pSTIR = (uint32_t *) 0xE000EF00;
	uint32_t *pISER0 = (uint32_t *) 0xE000E100;
	/* To enable the 3rd bit for interrupt no. 3 (RTC WKUP). */
	*pISER0 |= (1 << 3);
	/* To set software interrupt for IRQ3. */
	*pSTIR |= (3 & 0x1FF);
}

int main(void)
{
	uint32_t psp_val = 0x20008000;
	__asm__ volatile("MSR PSP, %0": :"r"(psp_val));
	__asm__ volatile("MSR CONTROL, %0": :"r"(0x2));
    printf("In thread mode : Before Interrupt.\n");
//	change_access_level_unpriv();					// changes to NPAL
    void (* fun)(void); // only the function name is the pointer
    fun = generate_interrupt;							// executes in PAL (Handler mode)
//    fun = (void *)0x080001dc;
    (*fun)(); // or fun();
    printf("In thread mode : After Interrupt.\n");
    for(;;);
}

void RTC_WKUP_IRQHandler(void) {
	printf("In handler mode...!\n");
}
