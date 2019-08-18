#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "timer.h"

#define F_CPU 128000UL
#include <util/delay.h>

#include "adc.h"

int main(void){
  uint8_t rv;
  DDRD = 0xFF;
  PORTD=0;
  adc_setup();
  PORTD=0x1;
  timer_setup();
  PORTD=0x02;
  sei();
  PORTD=0x9;

  while(1){
    adc_enable();
    rv=adc_start_conv(0);
    if(rv!=0){
      PORTD = 0x0F | rv;
      break;
    };  //do a conversion on channel 0, sleep while it is in progress
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
    PORTD = adc_get_last_value(); //output the result to PORTD
    adc_disable();
    //sleep until next timer ping
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
  }

  cli();
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_mode();
}
