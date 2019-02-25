// see http://www.avr-tutorials.com/interrupts/avr-external-interrupt-c-programming

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
	{ AVR_MCU_VCD_SYMBOL("PORTC"), .what = (void*)&PORTC, },
};
#endif

int is_running=1;

//see https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
ISR(TWI_vect){
  switch(TWSR){
    //slave receiver codes
    case 0x60:  //own address has been received, wake-up
      TWCR = TWCR | 0xC0; //set TWEA bit to ACK incoming byte
      break;
    case 0x68:  //arbitration lost, own address received
      break;
    case 0x70:  //general call address received
      break;
    case 0x78:  //arbitration lost, general call received
      break;
    case 0x80:  //own call address received, data ready, ACKED
      is_running = TWDR;  //receive data register into the is_running flag
      TWCR = TWCR | 0xC0; //set TWEA bit to ACK the incoming byte
      break;
    case 0x88:  //own call address received, data ready, NACKED
      break;
    case 0x90:  //general call address received, data ready, ACKED
      break;
    case 0x98:  //general call address received, data ready, NACKED
      break;
    case 0xA0:  //STOP condition or repeated START condition received
      TWCR = TWCR | 0xC0 & 0xCF; //set the TWEA bit, clear STA and STO to switch to not-addressed-listening
      break;
    default:
      break;
  }
}

void setup_twi(){
  //see p. 223 of datasheet
  //set up own address, and respond to global as well
  int8_t myaddress = 0x01;
  int8_t respondglobal = 0x01;

  TWAR = (myaddress << 1) | (respondglobal && 0x01);

  //set up Two-Wire Control Register
  TWCR = TWCR | 0xC4;  // 0xC4 = 11000100b, enable interrupt, enable acknowlege, enable TWI
  _delay_ms(50);
}

int main(void){
  sei();

  setup_twi();

  DDRC = 0x01;                       // initialize port C
  while(1)
  {
      // LED on
      if(is_running) PORTC = 0b00000001;            // PC0 = High = Vcc
      _delay_ms(50);                // wait 500 milliseconds

      //LED off
      if(is_running) PORTC = 0b00000000;            // PC0 = Low = 0v
      _delay_ms(50);                // wait 500 milliseconds
  }
}
