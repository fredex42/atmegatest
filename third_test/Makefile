MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR

all: third_test.hex test_pwm.hex testharness

.PHONY: testharness

testharness:
	make -C testharness

main.o: main.c
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

third_test.elf: main.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o third_test.elf main.o

third_test.hex: third_test.elf
	avr-objcopy -j .text -j .data -O ihex third_test.elf third_test.hex

test_pwm.o: test_pwm.c
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c test_pwm.c

test_pwm.elf: test_pwm.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o test_pwm.elf test_pwm.o

test_pwm.hex: test_pwm.elf
	avr-objcopy -j .text -j .data -O ihex test_pwm.elf test_pwm.hex

clean:
	rm -f *.o *.elf *.hex
	make -C testharness clean
