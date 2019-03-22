#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 128000UL
#include <avr/delay.h>

#include "adc.h"

uint8_t flags=ST_NONE;
uint8_t last_value=0x1;

ISR(ADC_vect){  //handle ADC interrupts
  last_value = ADCH;
  flags = flags &~ST_BUSY | ST_READY; //we have a value
}

void adc_setup(){
  ADMUX = (1 << REFS0) | (1<<ADLAR);  //set external reference and left-adjusted output. Left-adjustment means
                                      //we get 8-bit precision by only reading ADCH.
                                      //enable ADC and ADC interrupt
  adc_enable();
}

void adc_enable(){
  //disable for power-reduction ADC
  PRR = PRR & ~(1 << PRADC);
  //enable ADC and ADC interrupt
  ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS0);
  flags = flags & ~ST_DISABLE;
  _delay_ms(20);
}

void adc_disable(){
  //disable ADC
  ADCSRA = ADCSRA & ~(1<<ADEN);
  //enable power-reduction ADC
  PRR = PRR | (1 << PRADC);
  flags = flags | ST_DISABLE;
  _delay_ms(20);
}

uint8_t adc_start_conv(uint8_t channel){
  uint8_t channel_mask = (1<<3) | (1<<2) | (1<<1) | (1); //bottom 4 bits are channel id

  if(channel>8) return ERR_RANGE;
  if(flags&ST_BUSY) return ERR_BUSY;

  //select channel
  ADMUX &= ~channel_mask; //blank out the channel mask bits
  ADMUX = ADMUX | (channel & channel_mask);
  //start conversion
  ADCSRA = ADCSRA | (1<<ADSC);
  //flags=flags|ST_BUSY;
  return 0;
}

int8_t adc_get_last_value(){
  if(flags&ST_BUSY) return ERR_BUSY;
  flags = flags & ~ST_READY;
  return last_value;
}
