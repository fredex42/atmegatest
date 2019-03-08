//#include <avr/io.h>
#include <avr/interrupt.h>  //for sei();
#include <avr/sleep.h>

#define F_CPU 200000UL
#include <util/delay.h>

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

int main(void){
	DDRD = 0xFF;	//set all pins to output
	PORTD = 0x0;	//use PORTD to trace execution.

  setup_twi();
	PORTD = 0x01; //first LED when TWI is initialised

  sei();

	PORTD = 0x02;	//second LED when interrups enabled

  while(1){
		int8_t display_selector = twi_data_buffer[0];
		//PORTD = twi_bytes_received;
		PORTD = twi_data_buffer[display_selector];
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
  }
}
