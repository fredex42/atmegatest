#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL
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

  DDRD = 0x01;                       // initialize port C
  while(1)
  {
      // LED on
      PORTD = 0b00000001;            // PC0 = High = Vcc
      _delay_ms(500);                // wait 500 milliseconds

      //LED off
      PORTD = 0b00000000;            // PC0 = Low = 0v
      _delay_ms(500);                // wait 500 milliseconds
  }
}
