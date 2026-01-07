/*
 * gpio_driver.h
 *
 *  Created on: Oct 18, 2025
 *      Author: poojaa
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#define  __IO 	volatile

typedef struct {
	__IO uint32_t MODER0 : 2;
	__IO uint32_t MODER1 : 2;
	__IO uint32_t MODER2 : 2;
	__IO uint32_t MODER3 : 2;
	__IO uint32_t MODER4 : 2;
	__IO uint32_t MODER5 : 2;
	__IO uint32_t MODER6 : 2;
	__IO uint32_t MODER7 : 2;
	__IO uint32_t MODER8 : 2;
	__IO uint32_t MODER9 : 2;
	__IO uint32_t MODER10 : 2;
	__IO uint32_t MODER11 : 2;
	__IO uint32_t MODER12 : 2;
	__IO uint32_t MODER13 : 2;
	__IO uint32_t MODER14 : 2;
	__IO uint32_t MODER15 : 2;
} GPIOx_MODER;

typedef struct {
	__IO uint8_t OTYPER0 : 1;
	__IO uint8_t OTYPER1 : 1;
	__IO uint8_t OTYPER2 : 1;
	__IO uint8_t OTYPER3 : 1;
	__IO uint8_t OTYPER4 : 1;
	__IO uint8_t OTYPER5 : 1;
	__IO uint8_t OTYPER6 : 1;
	__IO uint8_t OTYPER7 : 1;
	__IO uint8_t OTYPER8 : 1;
	__IO uint8_t OTYPER9 : 1;
	__IO uint8_t OTYPER10 : 1;
	__IO uint8_t OTYPER11 : 1;
	__IO uint8_t OTYPER12 : 1;
	__IO uint8_t OTYPER13 : 1;
	__IO uint8_t OTYPER14 : 1;
	__IO uint8_t OTYPER15 : 1;
	__IO uint16_t reserved;
} GPIOx_OTYPER;

typedef struct {
	__IO uint8_t IDR0 : 1;
	__IO uint8_t IDR1 : 1;
	__IO uint8_t IDR2 : 1;
	__IO uint8_t IDR3 : 1;
	__IO uint8_t IDR4 : 1;
	__IO uint8_t IDR5 : 1;
	__IO uint8_t IDR6 : 1;
	__IO uint8_t IDR7 : 1;
	__IO uint8_t IDR8 : 1;
	__IO uint8_t IDR9 : 1;
	__IO uint8_t IDR10 : 1;
	__IO uint8_t IDR11 : 1;
	__IO uint8_t IDR12 : 1;
	__IO uint8_t IDR13 : 1;
	__IO uint8_t IDR14 : 1;
	__IO uint8_t IDR15 : 1;
	__IO uint16_t reserved;
} GPIOx_IDR;

typedef struct {
	__IO uint8_t ODR0 : 1;
	__IO uint8_t ODR1 : 1;
	__IO uint8_t ODR2 : 1;
	__IO uint8_t ODR3 : 1;
	__IO uint8_t ODR4 : 1;
	__IO uint8_t ODR5 : 1;
	__IO uint8_t ODR6 : 1;
	__IO uint8_t ODR7 : 1;
	__IO uint8_t ODR8 : 1;
	__IO uint8_t ODR9 : 1;
	__IO uint8_t ODR10 : 1;
	__IO uint8_t ODR11 : 1;
	__IO uint8_t ODR12 : 1;
	__IO uint8_t ODR13 : 1;
	__IO uint8_t ODR14 : 1;
	__IO uint8_t ODR15 : 1;
	__IO uint16_t reserved;
} GPIOx_ODR;

typedef struct {
	__IO uint32_t PUPDR0  : 2;
	__IO uint32_t PUPDR1  : 2;
	__IO uint32_t PUPDR2  : 2;
	__IO uint32_t PUPDR3  : 2;
	__IO uint32_t PUPDR4  : 2;
	__IO uint32_t PUPDR5  : 2;
	__IO uint32_t PUPDR6  : 2;
	__IO uint32_t PUPDR7  : 2;
	__IO uint32_t PUPDR8  : 2;
	__IO uint32_t PUPDR9  : 2;
	__IO uint32_t PUPDR10 : 2;
	__IO uint32_t PUPDR11 : 2;
	__IO uint32_t PUPDR12 : 2;
	__IO uint32_t PUPDR13 : 2;
	__IO uint32_t PUPDR14 : 2;
	__IO uint32_t PUPDR15 : 2;
} GPIOx_PUPDR;

typedef struct {
	__IO uint8_t GPIOAEN : 1;
	__IO uint8_t GPIOBEN : 1;
	__IO uint8_t GPIOCEN : 1;
	__IO uint8_t GPIODEN : 1;
	__IO uint8_t GPIOEEN : 1;
	__IO uint8_t GPIOFEN : 1;
	__IO uint8_t GPIOGEN : 1;
	__IO uint8_t GPIOHEN : 1;
	__IO uint8_t GPIOIEN : 1;
	__IO uint8_t reserved_1 : 3;
	__IO uint8_t CRCEN : 1;
	__IO uint8_t reserved_2 : 5;
	__IO uint8_t BKPSRAMEN : 1;
	__IO uint8_t reserved_3;
	__IO uint8_t CCMDATARAMEN : 1;
	__IO uint8_t DMA1EN : 1;
	__IO uint8_t DMA2EN : 1;
	__IO uint8_t reserved_4 : 2;
	__IO uint8_t ETHMACEN : 1;
	__IO uint8_t ETHMACTXEN : 1;
	__IO uint8_t ETHMACRXEN : 1;
	__IO uint8_t ETHMACPTPEN : 1;
	__IO uint8_t OTGHSEN : 1;
	__IO uint8_t OTGHSULPIEN : 1;
} RCC_AHB1ENR;

typedef struct {
	__IO uint32_t RCC_CR;
	__IO uint32_t RCC_PLLCFGR;
	__IO uint32_t RCC_CFGR;
	__IO uint32_t RCC_CIR;
	__IO uint32_t RCC_AHB1RSTR;
	__IO uint32_t RCC_AHB2RSTR;
	__IO uint32_t RCC_AHB3RSTR;
	__IO uint32_t reserved_1;
	__IO uint32_t RCC_APB1RSTR;
	__IO uint32_t RCC_APB2RSTR;
	__IO uint32_t reserved_2;
	__IO uint32_t reserved_3;
	__IO RCC_AHB1ENR RCC_AHB1ENR;
	__IO uint32_t RCC_AHB2ENR;
	__IO uint32_t RCC_AHB3ENR;
	__IO uint32_t reserved_4;
	__IO uint32_t RCC_APB1ENR;
	__IO uint32_t RCC_APB2ENR;
	__IO uint32_t reserved_5;
	__IO uint32_t reserved_6;
	__IO uint32_t RCC_AHB1LPENR;
	__IO uint32_t RCC_AHB2LPENR;
	__IO uint32_t RCC_AHB3LPENR;
	__IO uint32_t reserved_7;
	__IO uint32_t RCC_APB1LPENR;
	__IO uint32_t RCC_APB2LPENR;
	__IO uint32_t reserved_8;
	__IO uint32_t reserved_9;
	__IO uint32_t RCC_BDCR;
	__IO uint32_t RCC_CSR;
	__IO uint32_t reserved_10;
	__IO uint32_t reserved_11;
	__IO uint32_t RCC_SSCGR;
	__IO uint32_t RCC_PLLI2SCFGGR;
} GPIOx_RCC;

typedef struct {
	__IO GPIOx_MODER MODER;
	__IO GPIOx_OTYPER OTYPER;
	__IO uint32_t OSPEEDR;
	__IO GPIOx_PUPDR PUPDR;
	__IO GPIOx_IDR IDR;
	__IO GPIOx_ODR ODR;
	__IO uint32_t BSRR;
	__IO uint32_t LCKR;
	__IO uint32_t AFRL;
	__IO uint32_t AFRH;
} GPIO_PORT;

typedef struct {
	__IO GPIO_PORT *GPIOA;
	__IO GPIO_PORT *GPIOB;
	__IO GPIO_PORT *GPIOC;
	__IO GPIO_PORT *GPIOD;
	__IO GPIO_PORT *GPIOE;
	__IO GPIO_PORT *GPIOF;
	__IO GPIO_PORT *GPIOG;
	__IO GPIO_PORT *GPIOH;
	__IO GPIO_PORT *GPIOI;
} GPIOx;

#endif /* GPIO_DRIVER_H_ */
