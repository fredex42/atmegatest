MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR

all: pwm_test.hex

pwm.o: pwm.c
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c pwm.c

main.o: main.c pwm.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

pwm_test.elf: pwm.o main.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o pwm_test.elf main.o pwm.o

pwm_test.hex: pwm_test.elf
	avr-objcopy -j .text -j .data -O ihex pwm_test.elf pwm_test.hex

clean:
	rm -f *.o *.elf *.hex
