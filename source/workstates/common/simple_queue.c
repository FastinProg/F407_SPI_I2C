#include "simple_queue.h"


uint8_t	simple_queue_write(simple_queue_t* h, simple_queue_data_t *msg)
{
	uint16_t next = h->head + 1;

	if(next	>=	SIMPLE_QUEUE_MAX_SIZE)
		next = 0;

	if(next == h->tail)
		return 0;

	h->List[h->head] = *msg;
	h->head = next;

	return 1;
}


uint8_t simple_queue_is_empty(simple_queue_t* h)
{
    if(h->head == h->tail)
        return 1;
    else
    	return 0;
}


uint8_t simple_queue_read(simple_queue_t* h, simple_queue_data_t* msg)
{
    if(h->head == h->tail)
        return 0;

    *msg = h->List[h->tail];

    uint16_t next = h->tail + 1;
    if(next >= SIMPLE_QUEUE_MAX_SIZE)
        next = 0;

    h->tail = next;
    return 1;
}






