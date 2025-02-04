PROGRAM=test_arm
PROGRAM2=main

CC=arm-none-eabi-gcc
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu2x -O0

all:$(PROGRAM).o $(PROGRAM2).o
	@echo "Make complete..."	

$(PROGRAM).o:$(PROGRAM).c
	@$(CC) $(CFLAGS) $(PROGRAM).c -o $(PROGRAM).o
	@echo "Compiled $(PROGRAM).c ..."
	
$(PROGRAM2).o:$(PROGRAM2).c
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Compiled $(PROGRAM2).c ..."
	
clean:
	@rm -f $(PROGRAM).o $(PROGRAM2).o
	@echo "Make cleaned..."
