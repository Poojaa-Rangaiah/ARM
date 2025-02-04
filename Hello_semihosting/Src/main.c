#include <stdio.h>

extern void initialise_monitor_handles(void);

int main(void)
{
	initialise_monitor_handles();		// This method must be initialized before using any printf();

	printf("Hello! Semihosting s/w debugger with openOCD middleware is being tested...\n");	// w/o '\n' printf() doesn't print anything.

	for(;;);
}

/*
	Set the linker arguments in properties->C/C++ Build->settings->MCU GCC Linker->Miscellaneous->other flags
	-specs=rdimon.specs -lc -Irdimon

	Add semi-hosting run command in debugger->startup section.
	monitor arm semihosting enable

	Add the below function call to main.c
	extern void initialise monitor handles (void);
	initialise monitor handles ()

	Exclude syscalls.c from build project.
*/
