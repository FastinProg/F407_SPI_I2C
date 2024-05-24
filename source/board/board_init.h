#ifndef _BOARD_INIT
#define _BOARD_INIT
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "board_uart.h"
#include "board_i2c.h"

typedef enum{GPIO_ST_ON, GPIO_ST_OFF, GPIO_ST_TOGGLE}GPIO_State_e;

typedef enum {
	pin_input_drdy = (uint8_t)0,
	pin_input_dummy = (uint8_t)1,
}pin_in_e;

typedef enum {
	pin_out_cs_0 = (uint8_t)0,
}pin_out_e;

typedef struct
{
	GPIO_TypeDef*	GPIO_Port;
	GPIO_InitTypeDef GPIO_PinInit;
}stm32PinInit_t;

typedef struct
{
	GPIO_TypeDef*	GPIO_Port;
	GPIO_InitTypeDef GPIO_PinInit;
}stm32PinOut_t;

void Init_TIM6(void);
void Init_TIM7(void);
void GPIO_ExtInterpt(void);

void SystemClock_Config(void);
void board_init(void);

void hwSetDigitalOut(pin_out_e Pin_number, GPIO_State_e State);
uint8_t hwGetDigitalInput(pin_in_e Pin_number);
#endif

