//#include <avr/io.h>
#include <avr/interrupt.h>  //for sei();
#include <avr/sleep.h>

#define F_CPU 200000UL
#include <util/delay.h>

#define I2CADDRESS 0x03

#ifdef SIMULATOR
#include "avr/avr_mcu_section.h"  //need to ensure that simavr includes are on the path, e.g. pkg-config --cflags simavr
AVR_MCU(F_CPU, "atmega48");

//tell simavr to record the output of PORTC for gtkwave.
//this outputs a file called gtkwave_trace.vcd during the simulation run
const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
	{ AVR_MCU_VCD_SYMBOL("PORTD"), .what = (void*)&PORTD, },
	{ AVR_MCU_VCD_SYMBOL("TWCR"), .what = (void*)&TWCR, },
	{ AVR_MCU_VCD_SYMBOL("TWDR"), .what = (void*)&TWDR, },
	{ AVR_MCU_VCD_SYMBOL("TWAR"), .what = (void*)&TWAR, }
};
#endif

#include "twi.h"
#include "pwm.h"
#include "sensor_gpi.h"

char disabled_flag;

int main(void){
	DDRD = 0xFF;	//set all pins to output
	PORTD = 0x0;	//use PORTD to trace execution.

  setup_twi(I2CADDRESS);
	PORTD = 0x01; //first LED when TWI is initialised

	setup_pwm_0();
	PORTD = 0x02;	//second LED when PWM is initialised

	setup_sensor_gpi();

  sei();

	PORTD = 0x03;	//both LEDs when interrups enabled

  while(1){
		if(twi_data_byte<0x10){	//below this there is not enough power to turn the motor anyway
			disabled_flag=1;
			disable_pwm_0();
		} else {
			if(disabled_flag){
				disabled_flag=0;
				enable_pwm_0();
			}
			set_pwm_0b(twi_data_byte);
			set_pwm_0a(twi_data_byte);
		}
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
  }
}
