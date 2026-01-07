#include <stdint.h>
#include <stdio.h>

#if 0
#define OTG_HS_IRQNO  77
#define IRQ 		  OTG_HS_IRQNO
#define PEND_BASE	  0XE000E200
#define ENBL_BASE	  0xE000E100

int main(void)
{
	uint32_t *irqPND = (uint32_t *)(PEND_BASE + (IRQ / 32) * 4);
	uint32_t *irqEN = (uint32_t *)(ENBL_BASE + (IRQ / 32) * 4);

	/* Manually pend the interrupt in the interrupt status pending register (NVIC). */
	*irqPND |= (1 << (IRQ % 32));
	/* Set the interrupt set enable register for that particular interrupt. */
	*irqEN |= (1 << (IRQ % 32));

	for(;;);
}

// ISR OTG_HS_IRQHandler
void OTG_HS_IRQHandler(void) {
	printf("\n In OTG_HS_IRQHandler ISR.\n");
}
#endif
#if 0
#define TIM2_IRQNO    28
#define I2C1_IRQNO	  31
#define PEND_BASE	  0XE000E200
#define ENBL_BASE	  0xE000E100
#define PRIO_BASE	  0xE000E400

uint32_t *irqPND = (uint32_t *)PEND_BASE;
uint32_t *irqEN = (uint32_t *)ENBL_BASE;
uint32_t *irqPR = (uint32_t *)PRIO_BASE;

void irq_pri_config(uint8_t irq, uint8_t pri)
{
//	uint32_t *irqPRI = (uint32_t *)(PRIO_BASE + (irq / 4) * 4); // (irq / 4) * 4 for uint8_t -> the datatype is always important for address operations and not for data.

	uint32_t *irqPRI = (uint32_t *)(irqPR + (irq / 4)); // equivalent to irqPR++ or irqPR+1

	/* Clear and Set the priority value */
	*irqPRI &= ~(pri << ((irq % 4) * 8));
	*irqPRI |= (pri << ((irq % 4) * 8));

}

int main(void)
{
	uint32_t val = 0;

	irq_pri_config(I2C1_IRQNO, 0x60);
	irq_pri_config(TIM2_IRQNO, 0x70);

	/* Set the interrupt set enable register for that particular interrupt. */
	val = (1 << (TIM2_IRQNO % 32)) | (1 << (I2C1_IRQNO % 32));
	*irqEN = val;
	/* Pend the IRQ bit */
	*irqPND |= (1 << (TIM2_IRQNO % 32));

	for(;;);
}

void TIM2_IRQHandler(void) {
	printf("\n In TIM2_IRQHandler.\n");
	/* Pend the IRQ bit */
	*irqPND |= (1 << (I2C1_IRQNO % 32));
	while(1);
}
void I2C1_EV_IRQHandler(void) {
	printf("\n In I2C1_EV_IRQHandler.\n");
}
#endif

#define USART3 	 	39
#define TIM6_DAC 	54
#define OTG_HS_WKUP 76

#define PEND_BASE	  0XE000E200
#define ENBL_BASE	  0xE000E100
#define PRIO_BASE	  0xE000E400

uint32_t *irqPR = (uint32_t *)PRIO_BASE;

void irq_pri_config(uint8_t irq, uint8_t pri)
{
	uint32_t *irqPRI = (uint32_t *)(irqPR + (irq / 4)); // equivalent to irqPR++ or irqPR+1

	/* Clear and Set the priority value */
	*irqPRI &= ~(pri << ((irq % 4) * 8));
	*irqPRI |= (pri << ((irq % 4) * 8));
}

void irq_en(uint8_t irq) {
	uint32_t *irqEN = (uint32_t *)(ENBL_BASE + (irq / 32) * 4);
	*irqEN |= (1 << (irq % 32));
}

void irq_pend(uint8_t irq) {
	uint32_t *irqPND = (uint32_t *)(PEND_BASE + (irq / 32) * 4);
	*irqPND |= (1 << (irq % 32));
}

int main()
{
	irq_pri_config(USART3, 0x60);
	irq_pri_config(TIM6_DAC, 0x80);
	irq_pri_config(OTG_HS_WKUP, 0x70);

	irq_en(USART3);
	irq_en(TIM6_DAC);
	irq_en(OTG_HS_WKUP);

	irq_pend(TIM6_DAC);

	for(;;);
}

void USART3_IRQHandler(void)
{
	printf("USART3_IRQHandler\n");
	irq_pend(OTG_HS_WKUP);
	irq_pend(TIM6_DAC);
//	while(1);
}

void TIM6_DAC_IRQHandler(void)
{
	printf("TIM6_DAC_IRQHandler\n");
	irq_pend(OTG_HS_WKUP);
//	while(1);
}

void OTG_HS_WKUP_IRQHandler(void)
{
	printf("OTG_HS_WKUP_IRQHandler\n");
	irq_pend(USART3);
//	while(1);
}
