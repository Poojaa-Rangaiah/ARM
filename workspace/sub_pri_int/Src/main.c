#include <stdint.h>
#include <stdio.h>

#define USART3 	 	39
#define TIM6_DAC 	54

#define PEND_BASE	  0XE000E200
#define ENBL_BASE	  0xE000E100
#define PRIO_BASE	  0xE000E400
#define AIRCR_BASE	  0xE000ED0C

uint32_t *irqPR = (uint32_t *)PRIO_BASE;
uint32_t *AIRCR = (uint32_t *)AIRCR_BASE;
uint32_t *irqPND = (uint32_t *)(PEND_BASE + 4);

void irq_en(uint8_t irq) {
	uint32_t *irqEN = (uint32_t *)(ENBL_BASE + (irq / 32) * 4);
	*irqEN |= (1 << (irq % 32));
}

void irq_pend(uint8_t irq) {
	uint32_t *irqPND = (uint32_t *)(PEND_BASE + (irq / 32) * 4);
	*irqPND |= (1 << (irq % 32));
}

void irq_s_p_cfg(void) {
	/* VECTKEYSTAT, pri <7:6> and sub-pri <5:0> */
	*AIRCR = (0x5fa << 16) | (0x5 << 8);  // 2 levels of priority and 64 levels of sub-priority. But only 4 MSB bits are allocated for INT.
}

void irq_pri_config(uint8_t irq, uint8_t pri, uint8_t s_p)
{
	uint32_t *irqPRI = (uint32_t *)(irqPR + (irq / 4));

	*irqPRI &= ~(pri << ((irq % 4) * 8));
	*irqPRI |= ((((pri << 2) | s_p) << 4) << ((irq % 4) * 8));
}

int main()
{
	/* To configure sub-priority settings */
	irq_s_p_cfg(); // 2 levels of priority and 2 levels of sub-priority (only 4 MSB bits applicable)

	/* same priority */
	irq_pri_config(TIM6_DAC, 0x1, 0x1);	// 54
	irq_pri_config(USART3, 0x1, 0x1);	// 39

	irq_en(TIM6_DAC);
	irq_en(USART3);

	*irqPND = (1 << (USART3 % 32)) | (1 << (TIM6_DAC % 32));

	for(;;);
}

void TIM6_DAC_IRQHandler(void)
{
	printf("TIM6_DAC_IRQHandler\n");
//	while(1);
}

void USART3_IRQHandler(void)
{
	printf("USART3_IRQHandler\n");
//	while(1);
}
