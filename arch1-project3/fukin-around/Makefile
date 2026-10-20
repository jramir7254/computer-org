# Put this folder inside arch1-project3/ (next to wakedemo/),
# run "make" in arch1-project3 first so ../h and ../lib exist.
CPU     = msp430g2553
CFLAGS  = -mmcu=${CPU} -Os -I../h -I/opt/ti/msp430-gcc/include/
LDFLAGS = -L../lib -L/opt/ti/msp430-gcc/include

CC      = msp430-elf-gcc

all: main.elf

main.elf: main.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load: main.elf
	mspdebug tilib "prog $^"

clean:
	rm -f *.o *.elf
