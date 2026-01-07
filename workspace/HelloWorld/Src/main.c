//#include <stdint.h>
#include <stdio.h>
#if 1

int main() {
	printf("HELLO WORLD!");
	fflush(stdout); // can be used instead of \n for flushing the buffer.
	for(;;);
	return 0;
}

#else
// Access level demonstration of the ARM processor
void change_access_level_priv() {		// changing to PAL
	__asm volatile (
			"MRS R0, CONTROL\t\n"		//	Read
			"AND R0, R0, #0xFE\t\n"		//	Modify
			"MSR CONTROL, R0\t\n"		//	write
	);
}
void change_access_level_npriv() {		// changing to NPAL
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
    void (*fun)(void);
    fun = (void*)0x80001c8;					// changes to NPAL (hard coding the address of change_access_level_priv() such that the T-bit cannot be enabled here)
//    fun = (void*)change_access_level_priv;
    fun();
    printf("In thread mode : After Interrupt.\n");
    for(;;);
}
#endif
