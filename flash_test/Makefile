MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR

all: flash_test.hex

main.o: main.c
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

flash_test.elf: main.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o flash_test.elf main.o

flash_test.hex: flash_test.elf
	avr-objcopy -j .text -j .data -O ihex flash_test.elf flash_test.hex

clean:
	rm -f *.o *.elf *.hex
