MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR

all: adc_test.hex

adc.o: adc.c
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c adc.c

timer.o: timer.c
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c timer.c

main.o: main.c adc.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

adc_test.elf: adc.o main.o timer.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o adc_test.elf main.o adc.o

adc_test.hex: adc_test.elf
	avr-objcopy -j .text -j .data -O ihex adc_test.elf adc_test.hex

clean:
	rm -f *.o *.elf *.hex
