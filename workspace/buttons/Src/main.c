#include <stdint.h>

// unsigned integer pointer to RCC_AHB1ENR of RCC
uint32_t *RCC_AHB1ENR = (uint32_t *)0X40023830;
// unsigned integer pointer to GPIOx_MODER of GPIO PORT D
uint32_t *GPIOD_MODER = (uint32_t *)0x40020C00;
// unsigned integer pointer to GPIOx_MODER of GPIO PORT C
uint32_t *GPIOC_MODER = (uint32_t *)0x40020800;
// unsigned integer pointer to GPIOx_OTYPER of GPIO PORT D
uint32_t *GPIOD_OTYPER = (uint32_t *)0x40020C04;
// unsigned integer pointer to GPIOx_OTYPER of GPIO PORT C
uint32_t *GPIOC_OTYPER = (uint32_t *)0x40020804;
// unsigned integer pointer to GPIOx_PUPDR of GPIO PORT C
uint32_t *GPIOC_PUPDR = (uint32_t *)0x4002080c;
// unsigned integer pointer to GPIOx_ODR of GPIO PORT D
uint32_t *GPIOD_ODR = (uint32_t *)0x40020C14;
// unsigned integer pointer to GPIOx_IDR of GPIO PORT C
uint32_t *GPIOC_IDR = (uint32_t *)0x40020810;

void delay(int del) { while(--del); }

int main(void)
{
	*RCC_AHB1ENR = (1 << 2) | (1 << 3); 	// To disable the clock gating for PORT C and PORT D

	*GPIOD_MODER = (1 << 24) | (1 << 26); // To enable the output mode for PD12, PD13

	*GPIOC_MODER = (0 << 8) | (0 << 10) | (0 << 12); // To configure PORTC (4, 5, 6) to input mode

	*GPIOD_OTYPER = 0x0; 	// To configure PORTD to the push-pull state

	*GPIOC_OTYPER = (1 << 4) | (1 << 5) | (1 << 6); // To configure PORTC (4, 5, 6) to Pull Up/Pull Down state

	/*Assuming this register as buttons to pins*/
	*GPIOC_PUPDR = (2 << 8) | (2 << 10) | (2 << 12); // Setting only the PC6 to Pull Up and PC4, PC5 to Pull Down

	int i = 8;

	while(1)
	{
		*GPIOC_PUPDR &= ~(2 << i);
		*GPIOC_PUPDR |= (1 << i);

		if(*GPIOC_IDR & ((1 << 4) | (1 << 5) | (1 << 6)))  // To read only the PC4, PC5, PC6 in loop
		{
			if(i == 8) { // PC4
				*GPIOD_ODR = (1 << 12); // PD12, Green LED turns on
				*GPIOC_PUPDR &= ~(1 << i);
				*GPIOC_PUPDR |= (2 << i); // To reset PC4 to pull down state
				i = 10;
			}
			else if (i == 10) { // PC5
				*GPIOD_ODR = (1 << 13); // PD 13, Orange LED turns on
				*GPIOC_PUPDR &= ~(1 << i);
				*GPIOC_PUPDR |= (2 << i); // To reset PC5 to pull down state
				i = 12;
			}
			else if (i == 12) { // PC6
				*GPIOD_ODR = (1 << 12) | (1 << 13); // PD12 & PD13, Green and Orange LED turns on
				*GPIOC_PUPDR &= ~(1 << i);
				*GPIOC_PUPDR |= (2 << i); // To reset PC6 to pull down state
				i = 8;
			}
			delay(500000);
			*GPIOD_ODR = 0x0; // To reset the LED output
		}
	}
}
