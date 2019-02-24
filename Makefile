MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR

all: third_test.hex

main.o: main.c
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

third_test.elf: main.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o third_test.elf main.o

third_test.hex: third_test.elf
	avr-objcopy -j .text -j .data -O ihex third_test.elf third_test.hex

clean:
	rm -f *.o *.elf *.hex
