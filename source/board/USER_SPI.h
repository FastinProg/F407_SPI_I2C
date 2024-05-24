#ifndef _USER_SPI_H__H_
#define _USER_SPI_H_
#include "main.h"

void spi_init(void);
void spi_write_16_bit(uint16_t* p, uint8_t size);
void spi_write_8_bit(uint8_t* p, uint8_t size);
void spi_write_receive_8_bit(uint8_t* p, uint8_t* answer, uint8_t size);
#endif

