#include <stdint.h>
#if 1
#include "gpio_driver.h"

void delay(int del) { while(--del); }

int main() {

	GPIOx_RCC *RCC;						/* To configure RCC for PORT D */
	RCC = (GPIOx_RCC *)0x40023800;
	RCC->RCC_AHB1ENR.GPIODEN = 1;

	GPIOx GPIO1;						/* Initializing the address of PORT D */
	GPIO1.GPIOD = (GPIO_PORT *)0x40020C00;

	GPIO1.GPIOD->MODER.MODER12 = 1;		/* To configure MODERx for PORT D */
	GPIO1.GPIOD->MODER.MODER13 = 1;
	GPIO1.GPIOD->MODER.MODER14 = 1;
	GPIO1.GPIOD->MODER.MODER15 = 1;

	while (1)
	{
		GPIO1.GPIOD->ODR.ODR12 = 0x1;	// To make only PORT D <12> high
		GPIO1.GPIOD->ODR.ODR13 = 0;
		GPIO1.GPIOD->ODR.ODR14 = 0;
		GPIO1.GPIOD->ODR.ODR15 = 0;
		delay(500000);
		GPIO1.GPIOD->ODR.ODR12 = 0;		// To make only PORT D <13> high
		GPIO1.GPIOD->ODR.ODR13 = 0x1;
		GPIO1.GPIOD->ODR.ODR14 = 0;
		GPIO1.GPIOD->ODR.ODR15 = 0;
		delay(500000);
		GPIO1.GPIOD->ODR.ODR12 = 0;		// To make only PORT D <14> high
		GPIO1.GPIOD->ODR.ODR13 = 0;
		GPIO1.GPIOD->ODR.ODR14 = 0x1;
		GPIO1.GPIOD->ODR.ODR15 = 0;
		delay(500000);
		GPIO1.GPIOD->ODR.ODR12 = 0;		// To make only PORT D <15> high
		GPIO1.GPIOD->ODR.ODR13 = 0;
		GPIO1.GPIOD->ODR.ODR14 = 0;
		GPIO1.GPIOD->ODR.ODR15 = 0x1;
		delay(500000);
	}

	return 0;
}

#else
// unsigned integer pointer to RCC_AHB1ENR of RCC
uint32_t *RCC_AHB1ENR = (uint32_t *)0x40023830;
// unsigned integer pointer to GPIOx_MODER of GPIO PORT D
uint32_t *GPIOD_MODER = (uint32_t *)0x40020C00;
// unsigned integer pointer to GPIOx_OTYPER of GPIO PORT D
uint32_t *GPIOD_OTYPER = (uint32_t *)0x40020C04;
// unsigned integer pointer to GPIOx_ODR of GPIO PORT D
uint32_t *GPIOD_ODR = (uint32_t *)0x40020C14;

/* Delay block */
void delay(int del) { while(--del); }

int main(void)
{
	*RCC_AHB1ENR |= 0x08; 	// To disable the clock gating for PORT D
	*GPIOD_OTYPER = 0x0; 	// To configure it to the push-pull state
	*GPIOD_MODER |= 0x41000000 | 0x14000000; // To enable the output mode for PORT D <12> <15>

    /* Blink alternative LEDs */
	while (1)
	{
		*GPIOD_ODR = 0x1000;	// To make only PORT D <12> high
		delay(500000);
		*GPIOD_ODR = 0x2000;	// To make only PORT D <13> high
		delay(500000);
		*GPIOD_ODR = 0x4000;	// To make only PORT D <14> high
		delay(500000);
		*GPIOD_ODR = 0x8000;	// To make only PORT D <15> high
		delay(500000);
	}

	return 0;
}
#endif
