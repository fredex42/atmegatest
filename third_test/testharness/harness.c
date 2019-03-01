//adapted from https://github.com/buserror/simavr/blob/master/examples/board_i2ctest/i2ctest.c

#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <pthread.h>

#include "sim_avr.h"
#include "avr_twi.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"
#include "parts/i2c_eeprom.h"

avr_t * avr = NULL;
avr_vcd_t vcd_file;
avr_twi_t twi_port;

int main(int argc, char *argv[])
{
  elf_firmware_t f;
  const char *fname = "../third_test.elf";

  printf("Firmware path name is %s\n", fname);
  elf_read_firmware(fname, &f);

  printf("firmware %s freq=%d mmcu=%s\n", fname, (int)f.frequency, f.mmcu);

  avr = avr_make_mcu_by_name(f.mmcu);
  if(!avr){
    fprintf(stderr, "%s: AVR '%s' not known\n", fname, f.mmcu);
    exit(1);
  }
  avr_init(avr);
  avr_load_firmware(avr, &f);
  avr_twi_init(avr, &twi_port);
  
  avr->gdb_port = 1234;

  avr_vcd_init(avr, "gtkwave_output.vcd", &vcd_file, 100000 /* usec */);
  avr_vcd_add_signal(&vcd_file,
     avr_io_getirq(avr, AVR_IOCTL_TWI_GETIRQ(0), TWI_IRQ_STATUS),
     8 /* bits */,
     "TWSR"
   );

   printf( "\nLaunching:\n");

	int state = cpu_Running;
	while ((state != cpu_Done) && (state != cpu_Crashed))
    state = avr_run(avr);
}
