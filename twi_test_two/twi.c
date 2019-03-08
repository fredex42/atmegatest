#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 2000000UL
#include <util/delay.h>

#define I2CADDRESS 0x03
#include "twi.h"

int8_t twi_data_buffer[TWI_BUFFER_SIZE];
int16_t twi_current_index=0;
int16_t twi_bytes_received=0;

//see https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
//service the TWI (===I2C) interrupt
ISR(TWI_vect){
  switch(TWSR){
    //slave receiver codes
    case 0x60:  //own address has been received, wake-up
      twi_current_index = 0;  //start receiving more data
      TWCR = TWCR | (1<<TWINT) | (1 << TWEA); //set TWEA bit to ACK incoming byte
      //PORTD=0x01;
      break;
    case 0x68:  //arbitration lost, own address received. Unused since not using master mode
      TWCR = TWCR | (1<<TWINT) | (1<<TWEN);
      //PORTD=0x02;
      break;
    case 0x70:  //general call address received
      //PORTD=0x03;
      break;
    case 0x78:  //arbitration lost, general call received. Unused since not using master mode
      //PORTD=0x04;
      break;
    case 0x80:  //own call address received, data ready, ACKED
      twi_data_buffer[twi_current_index] = TWDR;  //receive data register into the is_running flag
      ++twi_current_index;
      ++twi_bytes_received;
      if(twi_current_index<TWI_BUFFER_SIZE){
        TWCR = TWCR | (1<<TWINT) | (1<<TWEA); //clear TWEA bit to ACK the incoming byte
      } else {
        TWCR = TWCR | (1<<TWINT) | (1<<TWEA) | (1<<TWSTO);  //abort the transfer if buffer is full
      }
      break;
    case 0x88:  //own call address received, data ready, NACKED
      //PORTD=0x05;
      break;
    case 0x90:  //general call address received, data ready, ACKED
      //PORTD=0x06;
      break;
    case 0x98:  //general call address received, data ready, NACKED
      //PORTD=0x07;
      break;
    case 0xA0:  //STOP condition or repeated START condition received
      //PORTD=0x08;
      TWCR = TWCR | (1<<TWINT) | (1<< TWEA); //set the TWEA bit, clear STA and STO to switch to not-addressed-listening
      break;
    //slave transmitter codes
    case 0xA8:  //own address has been received, ACK returned, need data byte.
      //PORTD=0x09;
      TWDR = twi_data_buffer[0];
      TWCR = TWCR | (1 << TWINT) | (0 << TWEA);  //this is the last data byte so TWEA should be 0
      break;
    case 0xB0:  //arbitration lost, then addressed for read. Not needed since we only operate as slave
      //PORTD=0x0A;
      break;
    case 0xB8:  //data byte transmitted, ACK received, load next byte.
      //PORTD=0x0B;
      //not needed when only working one-byte
      break;
    case 0xC0:  //data byte transmitted, NOT ACK received, stop
      //PORTD=0x0C;
      TWCR = TWCR | (1 << TWINT) | (1 << TWEA);  //switch to not addressed slave mode, will recognise own address
      break;
    case 0xC8:  //last data byte transmitted, ACK received.
      //PORTD=0x0D;
      TWCR = TWCR | (1 << TWINT) | (1 << TWEA);  //switch to not addressed slave mode, will recognise own address
      break;

    default:
      //PORTD=0xFF;
      TWCR = TWCR | (1 << TWINT) | (1 << TWEA);
      break;
  }
}

void setup_twi(){
//initialise buffer to zero
  memset(&twi_data_buffer,0,TWI_BUFFER_SIZE);

  //see p. 223 of datasheet
  //set up own address, and respond to global as well
  int8_t myaddress = I2CADDRESS;
  int8_t respondglobal = 0x00;

  TWAR = (myaddress << 1) | (respondglobal && 0x01);

  //set up Two-Wire Control Register
  TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
  _delay_ms(50);
}
