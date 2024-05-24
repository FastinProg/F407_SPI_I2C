#include "TIM6.h"
#include "main.h"
static uint32_t user_time = 0;


static void user_time_1ms (void);



void TIM7_IRQHandler()
{
	user_time_1ms();
	TIM7->SR&=~(1);	// сброс флага прерывания
}

static void user_time_1ms (void)
{
  user_time++;
}
uint32_t get_ms_time_from(uint32_t TimerNewStart)
{
	return user_time - TimerNewStart;
}

uint32_t get_ms_time (void)
{
  return user_time;
}
