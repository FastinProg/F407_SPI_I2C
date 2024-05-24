#ifndef SIMPLE_QUEUE_H_
#define SIMPLE_QUEUE_H_

#include <stdint.h>

#define	SIMPLE_QUEUE_MAX_SIZE	((uint8_t)10)
#define	SIMPLE_QUEUE_DATA_SIZE	((uint8_t)10)

#pragma pack(1)
typedef struct
{
	uint8_t data[SIMPLE_QUEUE_DATA_SIZE];
	uint8_t leght;
} simple_queue_data_t;
#pragma pack(4)

typedef struct
{
	simple_queue_data_t List[SIMPLE_QUEUE_MAX_SIZE];
	uint16_t head;
	uint16_t tail;
} simple_queue_t;


uint8_t	simple_queue_write(simple_queue_t* h, simple_queue_data_t *msg);
uint8_t simple_queue_is_empty(simple_queue_t* h);
uint8_t simple_queue_read(simple_queue_t* h, simple_queue_data_t* msg);

#endif
