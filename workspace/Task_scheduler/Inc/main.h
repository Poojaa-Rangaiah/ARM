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
#define IDLE_STACK_START (SRAM_END - 4*SIZE_TASK_STACK)
#define SCHED_STACK_START (SRAM_END - 5*SIZE_TASK_STACK)

#define HSI 16000000U
#define SYSTICK_TIMER_CLOCK	HSI
#define TICK_HZ 1000

#define MAX_TASKS	5 	// 4 user tasks + 1 idle task

#define DUMMY_xPSR	0x01000000U
#define EXC_RETURN	0xFFFFFFFD

#define SHCRS_MEMFAULT	 (1 << 16)
#define SHCRS_BUSFAULT	 (1 << 17)
#define SHCRS_USAGEFAULT (1 << 18)

#define TASK_READY_STATE 0x00
#define TASK_BLOCKED_STATE 0xFF

#define INTERRUPT_DISABLE()	__asm__ volatile ("MOV R0, #0x1\n\t" "MSR PRIMASK, R0\n\t")
#define INTERRUPT_ENABLE()    __asm__ volatile ("MOV R0, #0x0\n\t" "MSR PRIMASK, R0\n\t")

void enable_processor_fault(void);
void init_task_stack(void);
void init_systick_timer(uint32_t tick_hz);
__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack);
__attribute__((naked)) void switching_sp_to_psp(void);
void save_psp_value(uint32_t current_psp_val);
void update_next_task(void);
void update_g_ticks_count(void);
void unblock_tasks(void);

void task_delay(uint32_t ticks_count);

void idle_task(void);
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void idle_task(void) {
	while(1);
}

void task1_handler(void) {
	while(1) {
//		printf("1\n");
//		printf("In Task 1 handler...!\n");
		led_on(LED_GREEN);
		task_delay(1000);
		led_off(LED_GREEN);
		task_delay(1000); // will be in idle_task() until g_ticks_count = old g_ticks_count + ticks_count
	}
}

void task2_handler(void) {
	while(1) {
//		printf("2\n");
//		printf("In Task 2 handler...!\n");
		led_on(LED_ORANGE);
		task_delay(500);
		led_off(LED_ORANGE);
		task_delay(500);
	}
}

void task3_handler(void) {
	while(1) {
//		printf("3\n");
//		printf("In Task 3 handler...!\n");
		led_on(LED_RED);
		task_delay(250);
		led_off(LED_RED); // 250
		task_delay(250);
	}
}

void task4_handler(void) {
	while(1) {
//		printf("4\n");
//		printf("In Task 4 handler...!\n");
		led_on(LED_BLUE);
		task_delay(125); // 125,
		led_off(LED_BLUE); // 125
		task_delay(125); // 125+125,
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


/*
 * If the concept g_ticks_count still confuses you, try only having 2 tasks instead of 4 of same delay counts.
 * void task1_handler(void) {
	while(1) {
		led_on(LED_GREEN);	// , when g_ticks_count = 1000, when g_ticks_count = 2000
		task_delay(1000/2);
		led_off(LED_GREEN);	// when g_ticks_count = 500, when g_ticks_count = 1500, when g_ticks_count = 2500
		task_delay(1000/2); // will be in idle_task() until g_ticks_count = 1000 (== 500+ticks_count), until g_ticks_count = 1500 (== 1000+ticks_count)
	}
}

 * void task2_handler(void) {
	while(1) {
		led_on(LED_ORANGE);	// , when g_ticks_count = 1000, when g_ticks_count = 2000
		task_delay(500);
		led_off(LED_ORANGE);	// when g_ticks_count = 500, when g_ticks_count = 1500, when g_ticks_count = 2500
		task_delay(500);
	}
}
 * */
