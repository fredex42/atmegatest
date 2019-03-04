#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 128000000UL
#include <util/delay.h>

#define I2CADDRESS 0x01
#include "twi.h"

int8_t twi_data_byte=0x04;  //initial value, so something shows up on the port (third LED lit)

//see https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
//service the TWI (===I2C) interrupt
ISR(TWI_vect){
  switch(TWSR){
    //slave receiver codes
    case 0x60:  //own address has been received, wake-up
      TWCR = (1<<TWINT) | (1 << TWEA) | (1<<TWEN); //set TWEA bit to ACK incoming byte
      break;
    case 0x68:  //arbitration lost, own address received
      TWCR = (1<<TWINT) | (1<<TWEN);
      break;
    case 0x70:  //general call address received
      break;
    case 0x78:  //arbitration lost, general call received
      break;
    case 0x80:  //own call address received, data ready, ACKED
      twi_data_byte = TWDR;  //receive data register into the is_running flag
      TWCR = (1<<TWINT) | (1<<TWEA)| (1<<TWEN); //clear TWEA bit to ACK the incoming byte
      break;
    case 0x88:  //own call address received, data ready, NACKED
      break;
    case 0x90:  //general call address received, data ready, ACKED
      break;
    case 0x98:  //general call address received, data ready, NACKED
      break;
    case 0xA0:  //STOP condition or repeated START condition received
      TWCR = (1<<TWINT) | (1<< TWEA) | (1<<TWEN); //set the TWEA bit, clear STA and STO to switch to not-addressed-listening
      break;
    //slave transmitter codes
    case 0xA8:  //own address has been received, ACK returned, need data byte.
      TWDR = twi_data_byte;
      TWCR = (1 << TWINT) | (0 << TWEA)| (1<<TWEN);  //this is the last data byte so TWEA should be 0
      break;
    case 0xB0:  //arbitration lost, then addressed for read. Not needed since we only operate as slave
      break;
    case 0xB8:  //data byte transmitted, ACK received, load next byte.
      //not needed when only working one-byte
      break;
    case 0xC0:  //data byte transmitted, NOT ACK received, stop
      TWCR = (1 << TWINT) | (1 << TWEA)| (1<<TWEN);  //switch to not addressed slave mode, will recognise own address
      break;
    case 0xC8:  //last data byte transmitted, ACK received.
      TWCR = (1 << TWINT) | (1 << TWEA)| (1<<TWEN);  //switch to not addressed slave mode, will recognise own address
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
