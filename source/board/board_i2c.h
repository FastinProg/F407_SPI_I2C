/*
 * board_i2c.h
 *
 *  Created on: 21 мая 2024 г.
 *      Author: abodin
 */

#ifndef SOURCE_BOARD_BOARD_I2C_H_
#define SOURCE_BOARD_BOARD_I2C_H_
#include "stm32f4xx_hal_i2c.h"

uint8_t i2c_1_init(void);
uint8_t i2c_1_read_mem(uint8_t slave_address, uint32_t reg, int16_t register_len, uint8_t *data, int16_t lenght);
uint8_t i2c_1_write_mem(uint8_t slave_address, uint32_t reg, int16_t register_len, uint8_t *data, int16_t lenght);

uint8_t i2c_1_for_debug_address_with_read(uint8_t byte);
uint8_t i2c_1_for_debug_address_with_write(uint8_t byte);

#endif /* SOURCE_BOARD_BOARD_I2C_H_ */
