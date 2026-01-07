/*
 * main.h
 *
 *  Created on: Oct 29, 2025
 *      Author: poojaa
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stdio.h>
#include "led.h"

#define SIZE_TASK_STACK 1024U // 1KB
#define SIZE_SCHED_STACK 1024U // 1KB

#define SRAM_START 0x20000000U
#define SIZE_SRAM (128 * 1024)
#define SRAM_END (SRAM_START + SIZE_SRAM)

#define T1_STACK_START SRAM_END
#define T2_STACK_START (SRAM_END - SIZE_TASK_STACK)
#define T3_STACK_START (SRAM_END - 2*SIZE_TASK_STACK)
#define T4_STACK_START (SRAM_END - 3*SIZE_TASK_STACK)
#define SCHED_STACK_START (SRAM_END - 4*SIZE_TASK_STACK)

#define HSI 16000000
#define SYSTICK_TIMER_CLOCK	HSI
#define TICK_HZ 16000

#define MAX_TASKS	4

#define DUMMY_xPSR	0x01000000U
#define EXC_RETURN	0xFFFFFFFD

#define SHCRS_MEMFAULT	(1 << 16)
#define SHCRS_BUSFAULT	(1 << 17)
#define SHCRS_USAGEFAULT	(1 << 18)

void enable_processor_fault(void);
void init_task_stack(void);
void init_systick_timer(uint32_t tick_hz);
__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack);
__attribute__((naked)) void switching_sp_to_psp(void);
void save_psp_value(uint32_t current_psp_val);
void update_next_task(void);

void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void task1_handler(void) {
	while(1) {
//		printf("1\n");
//		printf("In Task 1 handler...!\n");
		led_on(LED_GREEN);
		delay(DELAY_COUNT_1S);
		led_off(LED_GREEN);
		delay(DELAY_COUNT_1S);
	}
}

void task2_handler(void) {
	while(1) {
//		printf("2\n");
//		printf("In Task 2 handler...!\n");
		led_on(LED_ORANGE);
		delay(DELAY_COUNT_500MS);
		led_off(LED_ORANGE);
		delay(DELAY_COUNT_500MS);
	}
}

void task3_handler(void) {
	while(1) {
//		printf("3\n");
//		printf("In Task 3 handler...!\n");
		led_on(LED_RED);
		delay(DELAY_COUNT_250MS);
		led_off(LED_RED);
		delay(DELAY_COUNT_250MS);
	}
}

void task4_handler(void) {
	while(1) {
//		printf("4\n");
//		printf("In Task 4 handler...!\n");
		led_on(LED_BLUE);
		delay(DELAY_COUNT_125MS);
		led_off(LED_BLUE);
		delay(DELAY_COUNT_125MS);
	}
}

void HardFault_Handler(void) {
	printf("Exception: In HardFault_Handler...!\n");
	while(1);
}

void BusFault_Handler(void) {
	printf("Exception: In BusFault_Handler...!\n");
	while(1);
}

void MemManage_Handler(void) {
	printf("Exception: In MemManage_Handler...!\n");
	while(1);
}

#endif /* MAIN_H_ */
