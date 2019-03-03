#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 128000000UL
#include <util/delay.h>

#define I2CADDRESS 0x01
#include "twi.h"

int8_t twi_data_byte=0x04;


//see https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
//service the TWI (===I2C) interrupt
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
      twi_data_byte = TWDR;  //receive data register into the is_running flag
      TWCR = TWCR | 0xC0; //clear TWEA bit to ACK the incoming byte
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
  int8_t myaddress = I2CADDRESS;
  int8_t respondglobal = 0x01;

  TWAR = (myaddress << 1) | (respondglobal && 0x01);

  //set up Two-Wire Control Register
  TWCR = TWCR | 0xC4;  // 0xC4 = 11000100b, enable interrupt, enable acknowlege, enable TWI
  _delay_ms(50);
}
