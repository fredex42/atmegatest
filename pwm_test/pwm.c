#include <avr/io.h>

/**
initialise PWM channel 2
*/
void setup_pwm_2(){
  //set data direction to OUT for port b pins 1,2 (OCR1A/OCR1B)
  DDRB = (1<<DDB3);
  DDRD = (1<<DDD3);

  OCR2A = 0xAA; //channel A half-power
  OCR2B = 0x01; //channel B 99% power

  TCNT2 = 0;
  TIMSK2 = 0;

  //clock select internal (128kHz) with 1024 divider => 125Hz or 8ms total
  TCCR2B = 0x5;
  //set control registers. Inverting mode fast PWM on channels A and B
  TCCR2A = 0xF1;
}

void set_pwm_2a(uint8_t value){
  OCR2A = value;
}

void set_pwm_2b(uint8_t value){
  OCR2B = value;
}
