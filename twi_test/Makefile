MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR

all: twi_test.hex

twi.o: twi.c
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c twi.c

main.o: main.c twi.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

twi_test.elf: twi.o main.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o twi_test.elf main.o twi.o

twi_test.hex: twi_test.elf
	avr-objcopy -j .text -j .data -O ihex twi_test.elf twi_test.hex

clean:
	rm -f *.o *.elf *.hex
