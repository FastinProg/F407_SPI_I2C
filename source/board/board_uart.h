#ifndef BOARD_UART_H_
#define BOARD_UART_H_
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_usart.h"

uint8_t uart_2_init(void);
void uart_thread(void);
void USART2_IRQHandler(void);

#endif
