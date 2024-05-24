/*
 * board_i2c.c
 *
 *  Created on: 21 мая 2024 г.
 *      Author: abodin
 */
#include "main.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_ll_i2c.h"

#define I2C_TIMEOUT							((uint32_t)1000)

static void _i2c_1_reset_state_bit_to_default(void);
static uint8_t _is_timeout(uint32_t start_tick, uint32_t timeout);
static uint8_t _i2c_1_is_busy(void);
static uint8_t _i2c_1_request_register(uint8_t slave_address, uint32_t reg, int16_t lenght);
static uint8_t _i2c_1_write_data(uint8_t slave_address, uint8_t *data, int16_t lenght);
static uint8_t _i2c_1_read_data(uint8_t slave_address, uint8_t *data, int16_t lenght);

typedef enum {
	I2C_DIRECTION_READ,
	I2C_DIRECTION_WRITE,
} i2c_direction_e;

uint8_t i2c_1_init(void) {
		__HAL_RCC_I2C1_CLK_ENABLE();
		/* I2C_SDA - PB9 */
		/* I2C_SLC - PB6 */
		GPIOB->MODER |= (MODE_AF << 12) | (MODE_AF << 18);
		GPIOB->OSPEEDR |= (GPIO_SPEED_FREQ_VERY_HIGH << 12) | (GPIO_SPEED_FREQ_VERY_HIGH << 18);
		GPIOB->OTYPER  |= (1 << 6) | (1 << 9);  // open drain
		GPIOB->AFR[0] |= (GPIO_AF4_I2C1 << 24);
		GPIOB->AFR[1] |= (GPIO_AF4_I2C1 << 4);

	  I2C_HandleTypeDef I2cHandle;
	  I2cHandle.Instance             = I2C1;

	  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	  I2cHandle.Init.ClockSpeed      = 400000;
	  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	  I2cHandle.Init.OwnAddress1     = 0x1;
	  I2cHandle.Init.OwnAddress2     = 0xFE;
	  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
	    return 1;

	  return 0;
}
static void _i2c_1_reset_state_bit_to_default(void) {
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
	LL_I2C_ClearFlag_ADDR(I2C1);
	LL_I2C_DisableBitPOS(I2C1);
}

static uint8_t _is_timeout(uint32_t start_tick, uint32_t timeout)
{
    if ((get_ms_time()- start_tick) >= timeout)
    {
    	LL_I2C_GenerateStopCondition(I2C1);
        return 1;
    }

    return 0;
}

static uint8_t _i2c_1_is_busy(void) {
	uint32_t time_stamp_ms = get_ms_time();
	/* Wait until line will be able to trnasmit / reseive */
	while (LL_I2C_IsActiveFlag_BUSY(I2C1)) {
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	return 0;
}

static uint8_t _i2c_1_request_register(uint8_t slave_address, uint32_t reg, int16_t lenght) {
	uint32_t time_stamp_ms = get_ms_time();
	int16_t index = 0;
	if(lenght <= 0)
		return 1;
	_i2c_1_reset_state_bit_to_default();
	LL_I2C_GenerateStartCondition(I2C1);
	/* Wait end of start condition */
	while (!LL_I2C_IsActiveFlag_SB(I2C1)) {
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	/* Try to find slave address on the bus */
	LL_I2C_TransmitData8(I2C1, I2C_7BIT_ADD_WRITE(slave_address));
	while (!LL_I2C_IsActiveFlag_ADDR(I2C1)) {
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	LL_I2C_ClearFlag_ADDR(I2C1);
	index = lenght - 1;
	while(index >= 0) {
		/* Request register */
		while (!LL_I2C_IsActiveFlag_TXE(I2C1)) {
			if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
				return 1;
		}
		/* Chose register */
		LL_I2C_TransmitData8(I2C1, (uint8_t)(reg >> (8 * index)));
		index--;
	}
	/* Request register */
	while (!LL_I2C_IsActiveFlag_TXE(I2C1)) {
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	return 0;
}


static uint8_t _i2c_1_write_data(uint8_t slave_address, uint8_t *data, int16_t lenght) {
	uint32_t time_stamp_ms = get_ms_time();
	_i2c_1_reset_state_bit_to_default();
//	LL_I2C_GenerateStartCondition(I2C1);
//	/* Wait end of start condition */
//	while (!LL_I2C_IsActiveFlag_SB(I2C1)) {
//		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
//			return 1;
//	}
//	/* Try to find slave address on the bus */
//	LL_I2C_TransmitData8(I2C1, I2C_7BIT_ADD_WRITE(slave_address));
//	while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
//	{
//		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
//			return 1;
//	}
//	LL_I2C_ClearFlag_ADDR(I2C1);
	while (lenght > 0) {
		while (!LL_I2C_IsActiveFlag_TXE(I2C1)) {
			if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
				return 1;
		}
		LL_I2C_TransmitData8(I2C1, *data);
		data++;
		lenght--;
		if(LL_I2C_IsActiveFlag_BTF(I2C1) && lenght != 0) {
			LL_I2C_TransmitData8(I2C1, *data);
			data++;
			lenght--;
		}
		while (!LL_I2C_IsActiveFlag_BTF(I2C1)) {
			if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
				return 1;
		}
	}
	LL_I2C_GenerateStopCondition(I2C1);
	return 0;
}

static uint8_t _i2c_1_read_data(uint8_t slave_address, uint8_t *data, int16_t lenght)
{
	uint32_t time_stamp_ms = get_ms_time();
	if(lenght == 0) {
		LL_I2C_GenerateStopCondition(I2C1);
		return 0;
	}
	_i2c_1_reset_state_bit_to_default();
	LL_I2C_GenerateStartCondition(I2C1);
	/* Wait end of start condition */
	while (!LL_I2C_IsActiveFlag_SB(I2C1))
	{
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	/* Try to find slave address on the bus */
	LL_I2C_TransmitData8(I2C1, I2C_7BIT_ADD_READ(slave_address));
	while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
	{
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	LL_I2C_ClearFlag_ADDR(I2C1);

	if (lenght == 1)
	{
		LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
		LL_I2C_GenerateStopCondition(I2C1);
	}
	else if (lenght == 2)
	{
		LL_I2C_EnableBitPOS(I2C1);
		LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
	}
	while (lenght > 0U)
	{
		if (lenght <= 3U)
		{
			if (lenght == 1)
			{
				while (!LL_I2C_IsActiveFlag_RXNE(I2C1))
				{
					if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
						return 1;
				}
				*data = LL_I2C_ReceiveData8(I2C1);
				data++;
				lenght--;
			}
			/* Two bytes */
			else if (lenght == 2U)
			{
				while (!LL_I2C_IsActiveFlag_BTF(I2C1))
				{
					if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
						return 1;
				}
				LL_I2C_GenerateStopCondition(I2C1);
				*data = LL_I2C_ReceiveData8(I2C1);
				data++;
				lenght--;
			}
			/* 3 Last bytes */
			else
			{
				while (!LL_I2C_IsActiveFlag_BTF(I2C1))
				{
					if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
						return 1;
				}
				/* Disable Acknowledge */
				LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
				/* Read data from DR */
				*data = LL_I2C_ReceiveData8(I2C1);
				data++;
				lenght--;
				/* Wait until BTF flag is set */
				while (!LL_I2C_IsActiveFlag_BTF(I2C1))
				{
					if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
						return 1;
				}
				/* Generate Stop */
				LL_I2C_GenerateStopCondition(I2C1);
				/* Read data from DR */
				*data = LL_I2C_ReceiveData8(I2C1);
				data++;
				lenght--;
				/* Read data from DR */
				*data = LL_I2C_ReceiveData8(I2C1);
				data++;
				lenght--;
			}
		}
		else
		{
			while (!LL_I2C_IsActiveFlag_RXNE(I2C1))
			{
				if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
					return 1;
			}
			*data = LL_I2C_ReceiveData8(I2C1);
			data++;
			lenght--;
			if (LL_I2C_IsActiveFlag_BTF(I2C1))
			{
				*data = LL_I2C_ReceiveData8(I2C1);
				data++;
				lenght--;
			}
		}
	}
        return 0;
}

uint8_t i2c_1_for_debug_address_with_read(uint8_t byte) {
	uint32_t time_stamp_ms = get_ms_time();
	_i2c_1_reset_state_bit_to_default();
	LL_I2C_GenerateStartCondition(I2C1);
	/* Wait end of start condition */
	while (!LL_I2C_IsActiveFlag_SB(I2C1))
	{
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	/* Try to find slave address on the bus */
	LL_I2C_TransmitData8(I2C1, I2C_7BIT_ADD_READ(byte));
	while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
	{
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	LL_I2C_ClearFlag_ADDR(I2C1);
	LL_I2C_GenerateStopCondition(I2C1);
	return 0;
}

uint8_t i2c_1_for_debug_address_with_write(uint8_t byte) {
	uint32_t time_stamp_ms = get_ms_time();
	_i2c_1_reset_state_bit_to_default();
	LL_I2C_GenerateStartCondition(I2C1);
	/* Wait end of start condition */
	while (!LL_I2C_IsActiveFlag_SB(I2C1))
	{
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	/* Try to find slave address on the bus */
	LL_I2C_TransmitData8(I2C1, I2C_7BIT_ADD_WRITE(byte));
	while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
	{
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	LL_I2C_ClearFlag_ADDR(I2C1);
	LL_I2C_GenerateStopCondition(I2C1);
	return 0;
}

uint8_t i2c_1_for_debug_read_1_byte(uint8_t byte) {
	uint32_t time_stamp_ms = get_ms_time();
	_i2c_1_reset_state_bit_to_default();
	LL_I2C_GenerateStartCondition(I2C1);
	/* Wait end of start condition */
	while (!LL_I2C_IsActiveFlag_SB(I2C1))
	{
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	/* Try to find slave address on the bus */
	LL_I2C_TransmitData8(I2C1, I2C_7BIT_ADD_READ(byte));
	while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
	{
		if (_is_timeout(time_stamp_ms, I2C_TIMEOUT))
			return 1;
	}
	LL_I2C_ClearFlag_ADDR(I2C1);
	LL_I2C_GenerateStopCondition(I2C1);
	return 0;
}

uint8_t i2c_1_read_mem(uint8_t slave_address, uint32_t reg, int16_t register_len, uint8_t *data, int16_t lenght) {
	uint32_t time_stamp_ms = get_ms_time();
	if(_i2c_1_is_busy())
		return 1;
	if(_i2c_1_request_register(slave_address, reg, register_len))
		return 1;
	if(_i2c_1_read_data(slave_address, data, lenght))
		return 1;
	return 0;
}

uint8_t i2c_1_write_mem(uint8_t slave_address, uint32_t reg, int16_t register_len, uint8_t *data, int16_t lenght) {
	uint32_t time_stamp_ms = get_ms_time();
	if(_i2c_1_is_busy())
		return 1;
	if(_i2c_1_request_register(slave_address, reg, register_len))
		return 1;
	if(_i2c_1_write_data(slave_address, data, lenght))
		return 1;
	return 0;
}


