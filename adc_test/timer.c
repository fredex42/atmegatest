#include <avr/io.h>
#include <avr/interrupt.h>

int8_t timer_wakeup=0;

//we are only using the timer for wake-up, so we don't need anything in the handler here
ISR(TIMER0_COMPA_vect){
  timer_wakeup=1;
}

ISR(TIMER0_COMPB_vect){

}

ISR(TIMER0_OVF_vect){

}

timer_setup(){
  //set up timer
  OCR0A = 0xFF; //count whole cycle

  //enable interrupts for compare match unit A
  TIMSK0 = (1<<OCIE0A) | (1<<TOIE0);

  //configure timer 0 for Clear Timer on Count (CTC) mode
  TCCR0A = (1<<WGM01);
  //TCCR0B = (1<<CS01); //divide by 8.  Next option is 64!
  TCCR0B = (1<<CS02);  //divide by 256
}
