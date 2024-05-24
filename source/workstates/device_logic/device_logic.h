#ifndef WORKSTATES_DEVISE_LOGIC_H_
#define WORKSTATES_DEVISE_LOGIC_H_

void device_logic(void);


typedef enum
{
  STATE_MACHINE_INITIAL,
} main_state_machine_e;

typedef struct
{
	simple_queue_t spi_queue;
	simple_queue_t usart_queue;
	main_state_machine_e state_machine;
} object_distonary_t;

#endif
