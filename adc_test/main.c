#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU 128000000UL
#include <util/delay.h>

#include "adc.h"

int main(void){
  DDRD = 0xFF;
  PORTD=0;
  adc_setup();
  PORTD=0x1;
  sei();
  PORTD=0x2;

  while(1){
    adc_enable();
    adc_start_conv(0);  //do a conversion on channel 0, sleep while it is in progress
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
    PORTD = adc_get_last_value(); //output the result to PORTD
    adc_disable();
    _delay_ms(1000);
  }
}