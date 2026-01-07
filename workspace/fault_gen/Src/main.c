#include <stdio.h>
#include <stdint.h>

#define SHCRS ((uint32_t *)0xE000ED24)
#define CFSR (uint32_t *)0xE000ED28
#define UFSR (unsigned short *)0xE000ED2A
#define CCR (uint32_t *)0xE000ED14

#define USAGEF_EN (1 << 18)
#define BUSF_EN (1 << 17)
#define MEMF_EN (1 << 16)

#define BUSF_PND (1 << 14)
#define MEMF_PND (1 << 13)
#define USAGEF_PND (1 << 12)
#define SVCall_PND (1 << 15)

#define BUSF_ACT	(1 << 1)
#define USAGEF_ACT	(1 << 3)
#define MEMF_ACT (1 << 0)
#define SVCall_ACT	(1 << 7)

#define UNALIGNED (1 << 3)
#define DIVBY0	(1 << 4)

int main(void)
{
//	*SHCRS |= (USAGEF_EN | BUSF_EN | MEMF_EN);
	*CCR |= DIVBY0;

/* Force processor to execute some undefined instructions */
#if 0
	/* Try with and without T bit */
	uint32_t *pSRAM = (uint32_t *)0x20010001; 	// random SRAM address
	*pSRAM = 0xFFFFFFFF;	// random opcode

	void (* fun)(void);
	fun = (void *)pSRAM;
	fun();	//executing a random function at pSRAM
#else
	uint32_t a = 100/0;
#endif
	for(;;);
}

__attribute__((naked)) void UsageFault_Handler(void) {
//	register uint32_t val; // such C code inside naked function won't let this function perform efficiently.
//	__asm__ ("MRS %0, MSP" : "=r"(val));
	__asm__ ("MRS r0, MSP");
	__asm__ ("B UsageFault_Handler_c"); // R0 is stored in the first argument.
}

void UsageFault_Handler_c(uint32_t *val_sf) {
// If the MSP is extracted here, the prolog and epilog sequences manipulate the MSP val of the SF,
// Thus the SF containing R0 is obtained modified thus MSP is extracted in naked function.
	printf("\nUsageFault_Handler \n");
	printf("\n UFSR = %lx\n\nMSP @ SF base = %lx\n", *UFSR, val_sf);
	printf("R0 = %lx\n", val_sf[0]);
	printf("R1 = %lx\n", val_sf[1]);
	printf("R2 = %lx\n", val_sf[2]);
	printf("R3 = %lx\n", val_sf[3]);
	printf("R12 = %lx\n", val_sf[4]);
	printf("LR = %lx\n", val_sf[5]);
	printf("PC = %lx\n", val_sf[6]);
	printf("xPSR = %lx\n", val_sf[7]);
	while(1);
}

void BusFault_Handler(void) {
	printf("\n BusFault_Handler \n");
//	while(1);
}

void MemManage_Handler(void) {
	printf("\n MemManage_Handler \n");
//	while(1);
}

void HardFault_Handler(void) {
	printf("\n HardFault_Handler \n");
	//	while(1);
}

void SVC_Handler(void) {
	printf("\n SVC_Handler \n");
	*SHCRS |= SVCall_PND;
}
