//extern globals
extern int8_t twi_data_buffer[];
extern int16_t twi_bytes_received;

//public functions from twi.c
void setup_twi();

#define TWI_BUFFER_SIZE   16
