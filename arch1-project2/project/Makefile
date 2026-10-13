# makfile configuration
COMMON_OBJECTS  = 
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../demos/h -I/opt/ti/msp430-gcc/include
LDFLAGS		= -L/opt/ti/msp430-gcc/include 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: buzzer.elf 

#additional rules for files
buzzer.elf: ${COMMON_OBJECTS} buzzer.o main.o ../demos/lib/libTimer.a
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

load: buzzer.elf
	mspdebug ${MSPLOADER_OPTION} "prog $^"

buzzer.c: buzzer.h
main.c: buzzer.h
clean:
	rm -f *.o *.elf



