#include <avr/io.h>
#define F_CPU 200000UL
#include <util/delay.h>
#include "sensor_gpi.h"

void setup_sensor_gpi()
{
  //sensor is on PORTC pin 0
  DDRC = 0x00;  //configure port C as output.
  PORTC = 0x01; //pull-up on pin 0
  _delay_ms(50); //delay 1 clockcycle as per datasheet, bodged for time being
}

/**
get the current value of the detector
*/
int8_t get_sensor_value()
{
  return PINC & PINC0;
}
