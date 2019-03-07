#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 200000UL	//this seems to give approximately the right output on the hardware with internal clo
#include <util/delay.h>

#ifdef SIMULATOR
#include "avr/avr_mcu_section.h"  //need to ensure that simavr includes are on the path, e.g. pkg-config --cflags simavr
AVR_MCU(F_CPU, "atmega48");

//tell simavr to record the output of PORTC for gtkwave.
//this outputs a file called gtkwave_trace.vcd during the simulation run
const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
	{ AVR_MCU_VCD_SYMBOL("PORTD"), .what = (void*)&PORTD, }
};
#endif

int main(void){
  sei();
	int8_t newvalue;
	int8_t direction=0;

  DDRD = 0xFF;                       // initialize port C
	newvalue=0x01;

  while(1)
  {
      // // LED on
      // PORTD = 0b00000001;            // PC0 = High = Vcc
      // _delay_ms(500);                // wait 500 milliseconds
      //
      // //LED off
      // PORTD = 0b00000000;            // PC0 = Low = 0v
			if(direction==0){
				newvalue = newvalue << 1;
				if(newvalue>=(1<<6)) direction=1;
			} else {
				newvalue = newvalue >> 1;
				if(newvalue==0x01) direction=0;
			}
			PORTD=newvalue;
      _delay_ms(500);                // wait 500 milliseconds
  }
}
