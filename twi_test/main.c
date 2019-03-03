//#include <avr/io.h>
#include <avr/interrupt.h>  //for sei();
#include <avr/sleep.h>

#define F_CPU 8000000UL
#include <util/delay.h>

#include "twi.h"

int main(void){
  setup_twi();

  sei();

  while(1){
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
  }
}
