#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU 8000000UL
#include <util/delay.h>

#ifdef SIMULATOR
#include "avr/avr_mcu_section.h"  //need to ensure that simavr includes are on the path, e.g. pkg-config --cflags simavr
AVR_MCU(F_CPU, "atmega48");

const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
	{ AVR_MCU_VCD_SYMBOL("PORTB"), .what = (void*)&PORTB, },
  { AVR_MCU_VCD_SYMBOL("PORTD"), .what = (void*)&PORTD, },
	{ AVR_MCU_VCD_SYMBOL("TCNT2"), .what = (void*)&TCNT2, },
  { AVR_MCU_VCD_SYMBOL("TCCR2A"), .what = (void*)&TCCR2A, },
	{ AVR_MCU_VCD_SYMBOL("TCCR2B"), .what = (void*)&TCCR2B, },
	{ AVR_MCU_VCD_SYMBOL("TIFR2"), .what = (void*)&TIFR2, }
};

#endif

ISR(TIMER0_COMPA_vect){

}

ISR(TIMER0_COMPB_vect){

}

ISR(TIMER0_OVF_vect){

}

void setup_pwm_0(){
  //set data direction to OUT for port b pins 1,2 (OCR1A/OCR1B)
  DDRB = (1<<DDB3);
  DDRD = (1<<DDD3);

  OCR2A = 0xAA; //channel A half-power
  OCR2B = 0x01; //channel B 3/4 power

  TCNT2 = 0;
  TIMSK2 = 0;

  //clock select internal (128kHz) with 1024 divider => 125Hz or 8ms total
  TCCR2B = 0x5;
  //set control registers. Inverting mode fast PWM on channels A and B
  TCCR2A = 0xF1;

}

int main(void){
  sei();
  setup_pwm_0();

  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_mode();
  // while(1){
  //   _delay_ms(1000);
  // }
}
