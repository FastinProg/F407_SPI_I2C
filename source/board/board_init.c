#include "board_init.h"
#include "main.h"

static void GPIO_Init(void);

const stm32PinInit_t GPIO_PinIn[] =
{
		{GPIOB, {(uint32_t)GPIO_PIN_11, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0} },		// pi_DRDY
		{GPIOB, {(uint32_t)GPIO_PIN_12, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0} }			// dummy
};

const stm32PinOut_t GPIO_PinOut[] =
{
		{GPIOD, {GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW, 0}},				// CS_0
};


void board_init(void)
{
  HAL_Init();
  SystemClock_Config();
  GPIO_Init();
  hwSetDigitalOut(pin_out_cs_0, GPIO_ST_ON);
  spi_init();
  Init_TIM7();
  uart_2_init();
  i2c_1_init();
}



static void GPIO_Init ()
{         /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOH_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        
	for(int i = 0; i < ARRAY_LEN(GPIO_PinOut); i++)
	{
          GPIO_InitTypeDef* p =(GPIO_InitTypeDef*)&GPIO_PinOut[i].GPIO_PinInit;
          HAL_GPIO_Init(GPIO_PinOut[i].GPIO_Port, p );
	}

	for(int i = 0; i < ARRAY_LEN(GPIO_PinIn); i++)
	{
          GPIO_InitTypeDef* p =(GPIO_InitTypeDef*)&GPIO_PinIn[i].GPIO_PinInit;  
          HAL_GPIO_Init(GPIO_PinIn[i].GPIO_Port, p );
	}

}

inline void hwSetDigitalOut(pin_out_e Pin_number, GPIO_State_e State)
{
	if(State == GPIO_ST_ON)
	{
		  GPIO_InitTypeDef* p =(GPIO_InitTypeDef*)&GPIO_PinOut[Pin_number].GPIO_PinInit;
		  HAL_GPIO_WritePin(GPIO_PinOut[Pin_number].GPIO_Port, p->Pin, GPIO_PIN_SET);
	}
	else if(State == GPIO_ST_OFF)
	{
		  GPIO_InitTypeDef* p =(GPIO_InitTypeDef*)&GPIO_PinOut[Pin_number].GPIO_PinInit;
		  HAL_GPIO_WritePin(GPIO_PinOut[Pin_number].GPIO_Port, p->Pin, GPIO_PIN_RESET);
	}
	else if(GPIO_ST_TOGGLE)
	{
		GPIO_InitTypeDef* p =(GPIO_InitTypeDef*)&GPIO_PinOut[Pin_number].GPIO_PinInit;
		HAL_GPIO_TogglePin(GPIO_PinOut[Pin_number].GPIO_Port, p->Pin);
	}
}

uint8_t hwGetDigitalInput(pin_in_e Pin_number)
{
	GPIO_InitTypeDef* p =(GPIO_InitTypeDef*)&GPIO_PinIn[Pin_number].GPIO_PinInit;
	uint8_t res = (uint8_t)HAL_GPIO_ReadPin(GPIO_PinIn[Pin_number].GPIO_Port, p->Pin);
	return res;
}






/*****************************************************************************************/
void Init_TIM7(void)
{
	RCC->APB1ENR |= (1<<5);		//ENABLE CLOCK APB1
	TIM7->DIER|= 0x0001;			// ENABLE REQUEST
	TIM7->PSC = 168 - 1; 				//1Мгц
	TIM7->ARR = 1000; 			// 1MГц / 1000 = 1мс
    TIM7->SR = 0x0000;
	TIM7->CR1 |= (1<<0) | (1<<7);//ENABLE CLOCK TIM6
	HAL_NVIC_SetPriority(TIM7_IRQn,3,4);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);// ENABLE IRQ
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure the main internal regulator output voltage

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  // Initializes the RCC Oscillators according to the specified parameters
  // in the RCC_OscInitTypeDef structure.

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  // Initializes the CPU, AHB and APB buses clocks

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}





