#include <stdint.h>
#include <stdio.h>

#define SRAM_START 0X20000000U
#define SRAM_SIZE (128 * 1024)
#define SRAM_END (SRAM_START + SRAM_SIZE)

#define STACK_START SRAM_END
#define STACK_MSP_START STACK_START
#define STACK_MSP_END (STACK_MSP_START + 512)
#define STACK_PSP_START STACK_MSP_END
#define STACK_PSP_END (STACK_PSP_START + 512)

/* We use naked function as it is faster than the normal function.*/
/* Here this asm code is to prevent compiler from doing the unstacking operation (AAPCS) if it is done, the
 * control will be taken over by the MSP by default but here we are shifting to PSP.*/

__attribute__((naked)) void change_psp_to_msp(void)
{
//	__asm volatile("LDR R0,=STACK_PSP_START");	// We cannot use a C macro in an assembly function. So we'll use equal assembler directives
//	__asm volatile("LDR R0, 0x1000000");
//	__asm volatile("MSR ESPR, R0");
	__asm volatile(".equ SRAM_END, (0X20000000 + (128 * 1024))"); /* .equ is the asm way of creating macros as C macros cannot be used here.*/
	__asm volatile(".equ PSP_START, (SRAM_END - 512)");
	__asm volatile("LDR R0,=PSP_START");
	__asm volatile("MSR PSP, R0");
	__asm volatile("MOV R0, #0X02");	// shows error if used LDR instead of MOV as LDR loads value from the address in spite of loading the immediate value.
	__asm volatile("MSR CONTROL, R0");	// To enable SPSEL=1 so SP shifts to PSP
	__asm volatile("BX LR");	/* Branch indirect to LR (link register) such that its content will be copied to the
								   PC (return address) so control comes back to the main() preventing MSP involvement here.*/
}

int fun_add(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	return a + b + c + d;
}

/* SVC is one of the exceptions of ARM Cortex-MX that is executed in thread mode to get some services in kernel level.
Used in OS environment when your application has separate kernel and user code to implement the system call layer.
It can be given a value between 0-255.*/
void generate_expression(void)
{
	__asm volatile("SVC #0x02");	// Calls the SVC exception handler in the thread mode.7
}


int main(void)
{
//	change_psp_to_msp();
	uint16_t ret;
	ret = fun_add(2, 4, 6, 8);
	printf("result = %d.",ret);
//	generate_expression();

	for(;;);
}

void SVC_Handler(void)
{	// Handler mode by default uses MSP. Once shifts to thread mode, makes use of PSP.
	printf("\nIn SVC handler.\n");
}
