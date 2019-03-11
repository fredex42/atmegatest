#include <avr/io.h>
#include <avr/interrupt.h>
// #define F_CPU 128000000UL
// #include <avr/delay.h>

#include "adc.h"

uint8_t flags=ST_NONE;
uint8_t last_value=0;

ISR(ADC_vect){  //handle ADC interrupts
  last_value = ADCH;
  flags = flags | ST_READY; //we have a value
}

void adc_setup(){
  ADMUX = (1 << REFS0) | (1<<ADLAR);  //set external reference and left-adjusted output. Left-adjustment means
                                      //we get 8-bit precision by only reading ADCH.
}

void adc_enable(){
  //disable for power-reduction ADC
  PRR = PRR & ~(1 << PRADC);
  //enable ADC and ADC interrupt
  ADCSRA = ADCSRA | (1<<ADEN) | (1<<ADIE);
  flags = flags & ~ST_DISABLE;
}

void adc_disable(){
  //disable ADC
  ADCSRA = ADCSRA & ~(1<<ADEN);
  //enable power-reduction ADC
  PRR = PRR | (1 << PRADC);
  flags = flags | ST_DISABLE;
}

uint8_t adc_start_conv(uint8_t channel){
  uint8_t channel_mask = (1<<3) | (1<<2) | (1<<1) | (1); //bottom 4 bits are channel id

  if(channel>8) return ERR_RANGE;
  if(flags|ST_BUSY) return ERR_BUSY;

  //select channel
  ADMUX = ADMUX | (channel & channel_mask);
  //start conversion
  ADCSRA = ADCSRA | (1<<ADSC);
}

int8_t adc_get_last_value(){
  if(flags|ST_BUSY) return ERR_BUSY;
  return last_value;
  flags = flags & ~ST_READY;
}
