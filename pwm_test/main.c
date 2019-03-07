#include <avr/io.h>
#include <avr/interrupt.h>  //for sei();
#include <avr/sleep.h>

#define F_CPU 2000000UL
#include <util/delay.h>

#ifdef SIMULATOR
#include "avr/avr_mcu_section.h"  //need to ensure that simavr includes are on the path, e.g. pkg-config --cflags simavr
AVR_MCU(F_CPU, "atmega48");

//tell simavr to record the output of PORTC for gtkwave.
//this outputs a file called gtkwave_trace.vcd during the simulation run
const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
	{ AVR_MCU_VCD_SYMBOL("PORTD"), .what = (void*)&PORTD, },
	{ AVR_MCU_VCD_SYMBOL("OCR2A"), .what = (void*)&OCR2A, },
	{ AVR_MCU_VCD_SYMBOL("TCNT2"), .what = (void*)&TCNT2, },
	{ AVR_MCU_VCD_SYMBOL("TCCR2A"), .what = (void*)&TCCR2A, },
  { AVR_MCU_VCD_SYMBOL("TCCR2B"), .what = (void*)&TCCR2B, },

};
#endif

#include "pwm.h"

int main(void){
	DDRD = 0xFF;	//set all pins to output
	PORTD = 0x0;	//use PORTD to trace execution.

  setup_pwm_2();
	PORTD = 0x01; //first LED when TWI is initialised

  sei();

	PORTD = 0x02;	//second LED when interrups enabled

  uint8_t c=0;

  while(1){
    c+=0x04;
    _delay_ms(500);
    if(c>0xF0) c=0;
    //PORTD=c;
    set_pwm_2b(c);
    //_delay_ms(1000);
  }
}
