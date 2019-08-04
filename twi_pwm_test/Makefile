MMCU_TYPE = atmega48
CFLAGS = `pkg-config --cflags simavr` -D SIMULATOR

all: twi_pwm_test.hex

twi.o: twi.c twi.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c twi.c

pwm.o: pwm.c pwm.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c pwm.c

sensor_gpi.o: sensor_gpi.c sensor_gpi.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c sensor_gpi.c

main.o: main.c twi.h pwm.h sensor_gpi.h
	avr-gcc -g -Os -mmcu=$(MMCU_TYPE) $(CFLAGS) -c main.c

twi_pwm_test.elf: twi.o main.o pwm.o sensor_gpi.o
	avr-gcc -g -mmcu=$(MMCU_TYPE) -o twi_pwm_test.elf main.o twi.o pwm.o sensor_gpi.o

twi_pwm_test.hex: twi_pwm_test.elf
	avr-objcopy -j .text -j .data -O ihex twi_pwm_test.elf twi_pwm_test.hex

clean:
	rm -f *.o *.elf *.hex
