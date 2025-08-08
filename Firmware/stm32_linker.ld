ENTRY(Reset_Handler)

MEMORY 
{
	FLASH(rx): ORIGIN =0x08000000, LENGTH =1024K
	SRAM(rwx): ORIGIN =0x20000000, LENGTH =128K
}

__max_heap_size = 0x400;
__max_stack_size = 0x200;

SECTIONS
{
	.text : /*to decide its storage of vma and lma; we won't relocate this section in the startup code as it will be stored in Flash (ROM).*/
	{
		*(.isr_vector) /* according to our flash, the section of vector table should be merged first.*/
		*(.text) /*.text main.o test_arm.o stm32f4_startup.o, where * tells to merge .text of all o/p files*/
		*(.rodata)
		_etext = .; /* Store the updated location value in the end_of_text symbol which can be used in .c files to represnt the address of end of .text*/
	}> FLASH  /* > FLASH(vma) AT> FLASH(lma)*/
	
	.data :
	{
		_sdata = .; /* The location counter resets to start of vma of this sec so it will be SRAM's addrs. (Note: always location counter tracks the vma of the sec)*/
		*(.data)
		_edata = .;
	}> SRAM AT> FLASH
	
	.bss :
	{
		_sbss = .;
		*(.bss)
		_ebss = .;
	}> SRAM
}
