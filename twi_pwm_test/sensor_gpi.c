#include <avr/io.h>
#include "sensor_gpi.h"

void setup_sensor_gpi()
{
  //sensor is on PORTC pin 0
  DDRC = 0x00;  //configure port C as output.
  PORTC = 0x01; //pull-up on pin 0
  __no_operation(); //delay 1 clockcycle as per datasheet
}

/**
get the current value of the detector
*/
int8_t get_sensor_value()
{
  return PINC & PINC0;
}
