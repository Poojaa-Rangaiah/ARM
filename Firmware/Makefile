PROGRAM=test_arm
PROGRAM2=main

CC=arm-none-eabi-gcc
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu2x -Wall -O0 # -std is the GNU C std, -Wall = turns on all the warnings.

LDFLAGS= -nostdlib -T stm32_linker.ld -Wl,-Map=final.map #-Wl = intimates it's a linker cmd altho -gcc is used instead of -ld, -Map generates a memory mapped file

all:Mkdir $(PROGRAM).o $(PROGRAM2).o stm32f4_startup.o final.elf dump
	@echo "Make complete..."	

Mkdir:
	@mkdir -p ./output

$(PROGRAM).o:$(PROGRAM).c
	@$(CC) $(CFLAGS) $(PROGRAM).c -o ./output/$(PROGRAM).o
	@echo "Compiled $(PROGRAM).c"
	
$(PROGRAM2).o:$(PROGRAM2).c
	@$(CC) $(CFLAGS) $^ -o ./output/$@
	@echo "Compiled $(PROGRAM2).c"
	
stm32f4_startup.o:stm32f4_startup.c
	@$(CC) $(CFLAGS) stm32f4_startup.c -o ./output/stm32f4_startup.o
	@echo "Compiled the startup file."
	
final.elf: ./output/$(PROGRAM).o ./output/$(PROGRAM2).o ./output/stm32f4_startup.o
	@$(CC) $(LDFLAGS) $^ -o ./output/$@
	@echo "Linked and .elf file generated."
	
dump:
	arm-none-eabi-objdump -d ./output/final.elf > ./output/final.dump
	
clean:
	@#@rm -f $(PROGRAM).o $(PROGRAM2).o stm32f4_startup.o
	@rm -f *.o *.elf
	@rm -r output
	@echo "Make cleaned..."
