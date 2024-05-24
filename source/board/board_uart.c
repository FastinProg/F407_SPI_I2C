#include "main.h"
#include "simple_queue.h"

#define USART_BUFF_MAX_SIZE ((uint8_t)10)


uint8_t buff[USART_BUFF_MAX_SIZE];
volatile uint8_t head;
volatile uint8_t tail;

uint8_t uart_2_init(void) {
	__HAL_RCC_USART2_CLK_ENABLE();
	/* USART TX - PA2 */
	/* USART RX - PA3 */
	GPIOA->MODER |= (MODE_AF << 4) | (MODE_AF << 6);
	GPIOA->OSPEEDR |= (GPIO_SPEED_FREQ_VERY_HIGH << 4) | (GPIO_SPEED_FREQ_VERY_HIGH << 6);
	GPIOA->AFR[0] |= (GPIO_AF7_USART1 << 8) | (GPIO_AF7_USART1 << 12);
    LL_USART_InitTypeDef config;
    config.BaudRate = 115200;
    config.DataWidth = LL_USART_DATAWIDTH_8B;
    config.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    config.OverSampling = LL_USART_OVERSAMPLING_16;
    config.Parity = LL_USART_PARITY_NONE;
    config.StopBits = LL_USART_STOPBITS_1;
    config.TransferDirection = LL_USART_DIRECTION_TX_RX;
    ErrorStatus rez = LL_USART_Init(USART2, &config);
    if(rez != SUCCESS)
    	return 1;

    USART2->CR1 |= (1 << 5);	/* enable receive IRQ */
    LL_USART_Enable(USART2);
    head = 0;
    tail = 0;
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  return 0;
}

void uart_thread(void) {
	while(head != tail) {
		simple_queue_data_t queue_item;
		queue_item.data[0] = buff[tail];
		queue_item.leght = 1;
		if(++tail >= USART_BUFF_MAX_SIZE)
			tail = 0;

		simple_queue_write(&od.usart_queue, &queue_item);
	}
}

void USART2_IRQHandler(void) {
	if(USART2->SR & (1 << 5)) {
                buff[head] = USART2->DR;
		if(++head >= USART_BUFF_MAX_SIZE)
			head = 0;
		
	}
}


