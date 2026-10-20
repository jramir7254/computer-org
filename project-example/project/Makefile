SRC  = project_main.o state_machine.o timer.o switches.o buzzer.o led.o
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h -I/opt/ti/msp430-gcc/include/
LDFLAGS = ../demos/lib/libTimer.a -L/opt/ti/msp430-gcc/include/

# Switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: arch_project_1.elf 

# Additional rules for files
arch_project_1.elf: ${SRC} ../demos/lib/libTimer.a
	${CC} $(CFLAGS) $(LDFLAGS) -o $@ $^

load: arch_project_1.elf
	mspdebug tilib "prog $^"

clean:
	rm -f *.o *.elf
