#include <stdint.h>
#if 1
#include <gpio_driver.h>

int main(void)
{
	GPIOx_RCC *RCC = (GPIOx_RCC *)0x40023800;
	/* Enabling clock for PORTA and PORTD */
	RCC->RCC_AHB1ENR.GPIOAEN = 1;
	RCC->RCC_AHB1ENR.GPIODEN = 1;

	GPIOx GPIO;
	GPIO.GPIOA = (GPIO_PORT*)0x40020000;
	GPIO.GPIOD = (GPIO_PORT *)0x40020C00;
	/* Configuring PORTA as input */
	GPIO.GPIOA->MODER.MODER0 = 0x0; 	// PA0 as input
	GPIO.GPIOA->PUPDR.PUPDR0 = 2;		// setting pull down for PA0 (mandatory for input configuration)
	/* Configuring PORTD as output */
	GPIO.GPIOD->MODER.MODER12 = 0x1;	// PD12 as output
	GPIO.GPIOD->MODER.MODER13 = 0x1;	// PD13 as output
	GPIO.GPIOD->MODER.MODER14 = 0x1;	// PD14 as output
	GPIO.GPIOD->MODER.MODER15 = 0x1;	// PD15 as output

	/* Polling IDR0 for button press */
	while(1)
	{
		switch(GPIO.GPIOA->IDR.IDR0)
		{
		case 1:							// If button is pressed, PA0 is high
			GPIO.GPIOD->ODR.ODR12 = 0x1;// ON PD12 LED
			GPIO.GPIOD->ODR.ODR13 = 0x1;
			GPIO.GPIOD->ODR.ODR14 = 0x1;
			GPIO.GPIOD->ODR.ODR15 = 0x1;
			while(GPIO.GPIOA->IDR.IDR0);// Wait until the button is released
			break;
		default:
			GPIO.GPIOD->ODR.ODR12 = 0x0;// Have PD12 LED OFF by default
			GPIO.GPIOD->ODR.ODR13 = 0x0;
			GPIO.GPIOD->ODR.ODR14 = 0x0;
			GPIO.GPIOD->ODR.ODR15 = 0x0;
		}
	}
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
// unsigned integer pointer to GPIOx_IDR of GPIO PORT A
uint32_t *GPIOA_IDR = (uint32_t *)0x40020010;
// unsigned integer pointer to GPIOx_MODER of GPIO PORT A
uint32_t *GPIOA_MODER = (uint32_t *)0x40020000;
// unsigned integer pointer to GPIOx_PUPDR of GPIO PORT A
uint32_t *GPIOA_PUPDR = (uint32_t *)0x4002000c;
// unsigned integer pointer to GPIOx_BSRR of GPIO PORT D
uint32_t *GPIOD_BSRR = (uint32_t *)0x40020C18;

int main()
{
	*RCC_AHB1ENR |= (1 << 0) | (1 << 3);	// disabling clock gating for PORTA and PORTD
	*GPIOD_MODER |= (1 << 24);  			// Setting PORTD as output
	*GPIOD_OTYPER = 0x0;					// setting PORTD to push-pull mode
	*GPIOA_MODER &= ~(3 << 0); 				// setting PORTA to input mode
	*GPIOA_PUPDR |= (2 << 0);				// setting PULL DOWN for PORT A

	while(1) 								// polling for button press at PA0
	{
		if((*GPIOA_IDR & 0x1) == 0x01) 		// checking for a button press
		{
//			*GPIOD_ODR |= (1 << 12);		// enabling PORTD pin 12
			*GPIOD_BSRR |= (1 << 12);		// setting the bit set reset register for enabling PORTD pin<12>
			while((*GPIOA_IDR & 0x1) == 0x01); // waiting till detecting button press
		}
//		*GPIOD_ODR &= ~(1 << 12);			// disabling PORTD pin 12
//		*GPIOD_BSRR &= ~(1 << 12);			// didn't work
		*GPIOD_BSRR |= (1 << 28);		    // setting the bit set reset register for resetting PORTD pin<12>
	}
}
#endif
