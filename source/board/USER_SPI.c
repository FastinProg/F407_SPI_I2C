#include "USER_SPI.h"

uint8_t dma_rx_buf[3];
uint8_t dma_rx1_buf[3];


void spi_init(void)
{
	//DMA_Init();
	// PB5 - MOSI
	// PB4 - MISO
	// PB3 - CLK
	RCC->AHB1ENR |= (1 << 1);
	GPIOB->MODER |= (2 << 6) | (2 << 8) | (2 << 10);
	GPIOB->PUPDR = 0;
	GPIOB->OSPEEDR |= (3 << 6) | (3 << 8) | (3 << 10);
	GPIOB->AFR[0] |= (6 << 12) | (6 << 16) | (6 << 20);

	RCC->APB1ENR |= (1 << 15);
	HAL_NVIC_DisableIRQ(SPI3_IRQn);
	SPI3->CR1 = 0;
	SPI3->CR1 |= (0 << SPI_CR1_DFF_Pos)  		// 0 - 8 bit, 1 - 16 bit
			| (0 << SPI_CR1_LSBFIRST_Pos)     			// 0 - MSB, 1 - LSB
			| (1 << SPI_CR1_SSM_Pos)          	// Software use CS
			| (1 << SPI_CR1_SSI_Pos)         	// Software use CS
			| (3 << SPI_CR1_BR_Pos)         	// F_PCLK / 16
			| (1 << SPI_CR1_MSTR_Pos)         	// MASTER MODE
			| (0 << SPI_CR1_CPOL_Pos) | (1 << SPI_CR1_CPHA_Pos);

	SPI3->CR2 = 0;
	//SPI3->CR2 |= 1 << SPI_CR2_RXDMAEN_Pos;		// Enable DMA
	SPI3->CR1 |= 1 << SPI_CR1_SPE_Pos; 			// Enable SPI
}

void spi_write_8_bit(uint8_t *p, uint8_t size)
{
	uint8_t tx_size = size;

	if (tx_size == 1)
	{
		SPI3->DR = p[size - tx_size];
		tx_size--;
		while (!(SPI3->SR & (SPI_SR_TXE)))
		{
		}
	}

	while (tx_size > 0U)
	{
		/* Wait until TXE flag is set to send data */
		if (SPI3->SR & (SPI_SR_TXE))
		{
			SPI3->DR = p[size - tx_size];
			tx_size--;
		}
	}
	while ((SPI3->SR & (SPI_SR_BSY)))
	{
	}
}

void spi_write_receive_8_bit(uint8_t *p, uint8_t *answer, uint8_t size)
{
	uint8_t FirstTranmit = 1;
	uint8_t cnt = 0;

	uint8_t tx_size = size;
	uint8_t rx_size = size;
	uint8_t txallowed = 1;

	while ((tx_size > 0U) || (rx_size > 0U))
	{
		/* Check TXE flag */
		if ((SPI3->SR & (SPI_SR_TXE)) && (tx_size > 0U) && (txallowed == 1U || cnt == 1))
		{
			cnt++;
			SPI3->DR = p[size - tx_size];
			tx_size--;
			/* Next Data is a reception (Rx). Tx not allowed */
			txallowed = 0U;
		}

		/* Wait until RXNE flag is reset */
		if ((SPI3->SR & (SPI_SR_RXNE)) && (rx_size > 0U))
		{
			answer[size - rx_size] = SPI3->DR;
			rx_size--;
			/* Next Data is a Transmission (Tx). Tx is allowed */
			txallowed = 1U;
			FirstTranmit = 0;
		}

	}
	while ((SPI3->SR & (SPI_SR_BSY)))	{}
	while (SPI3->SR & (SPI_SR_RXNE) == 0)	{}
	answer[size - rx_size] = SPI3->DR;

}

//void SPI_WriteRecive_8BIT(uint8_t *p, uint8_t *answer, uint8_t size)
//{
//	uint8_t tx_size = size;
//	uint8_t rx_size = size;
//	uint8_t total_size = size;
//	uint8_t txallowed = 1;
//
//	while ((tx_size > 0U) || (rx_size > 0U))
//	{
//		/* Check TXE flag */
//		if ((SPI3->SR & (SPI_SR_TXE)) && (tx_size > 0U) && (txallowed == 1U))
//		{
//			SPI3->DR = p[size - tx_size];
//			tx_size--;
//			/* Next Data is a reception (Rx). Tx not allowed */
//			txallowed = 0U;
//		}
//		/* Wait until RXNE flag is reset */
//		if ((SPI3->SR & (SPI_SR_RXNE)) && (rx_size > 0U))
//		{
//			answer[size - rx_size] = SPI3->DR;
//			rx_size--;
//			/* Next Data is a Transmission (Tx). Tx is allowed */
//			txallowed = 1U;
//		}
//	}
//	while ((SPI3->SR & (SPI_SR_BSY)))
//	{
//	}
//}

void spi_write_16_bit(uint16_t *p, uint8_t size)
{
	SPI3->DR = p[0];
	while (!(SPI3->SR & (SPI_SR_TXE)))		// Wait TXE ready
	{
	}
	for (uint8_t i = 1; i < size; i++)
	{
		SPI3->DR = p[i];							// Write SECOND BYTE
		while (!(SPI3->SR & (SPI_SR_RXNE)))
		{
		}
	}
	while ((SPI3->SR & (SPI_SR_BSY)))
	{
	}
}

void spi_write_receive_16_bit(uint16_t *p, uint16_t *answer, uint8_t size)
{
	//cs_low;
	SPI3->DR = p[0];
	while (!(SPI3->SR & (SPI_SR_TXE)))		// Wait TXE ready
	{
	}
	for (uint16_t i = 1; i < size; i++)
	{
		SPI3->DR = p[i];							// Write SECOND BYTE
		while (!(SPI3->SR & (SPI_SR_RXNE)))
		{
		}
		*answer = SPI3->DR;
		answer++;
	}
	*answer = SPI3->DR;
	while ((SPI3->SR & (SPI_SR_BSY)))
	{
	}
}
