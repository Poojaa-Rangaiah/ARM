#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

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
	*pISER0 |= 1 << 3;
	*pSTIR = 3 & 0x1FF;
}
void HardFault_Handler() {			// executed when SFR is attempted in NPAL
	printf("Hard fault detected.\n");
	while(1);
}
int main(void)
{
    printf("In thread mode : Before Interrupt.\n");
    change_access_level_unpriv();					// changes to NPAL
    generate_interrupt();							// executes in PAL (Handler mode)
    printf("In thread mode : After Interrupt.\n");
    for(;;);
}
