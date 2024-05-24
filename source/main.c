#include "main.h"
#include "stdint.h"


// Main
int main(void)
{
	// HW_Init
	board_init();
	device_logic();
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}
