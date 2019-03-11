//we are only using the timer for wake-up, so we don't need anything in the handler here
ISR(TIMER0_COMPA_vect){

}

setup_display(){
  //set up timer
  OCR0A = 0xFF; //count whole cycle

  //enable interrupts for compare match unit A
  TIMSK = (1<<OCIE0A) | (1<<T0IE0);

  //configure timer 0 for Clear Timer on Count (CTC) mode
  TCCR0A = (1<<WGM01);
  //TCCR0B = (1<<CS01); //divide by 8.  Next option is 64!
  TCCR0B = (1<<CS02) | (1<<CS00); //divide by 1024
}
