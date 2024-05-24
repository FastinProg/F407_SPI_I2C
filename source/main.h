#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG


#define ARRAY_LEN(a)	(sizeof(a)/sizeof(a[0]))



#include "stdint.h"
#include "simple_queue.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include "user_typedef.h"
#include "TIM6.h"
#include "USER_SPI.h"
#include "board_init.h"

#include "device_logic.h"



void Error_Handler(void);
  
extern object_distonary_t od;


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
