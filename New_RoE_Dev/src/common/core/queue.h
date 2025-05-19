/*
 * queue.h
 *
 * Created: 2025-05-16 오후 5:58:18
 *  Author: RCY
 */ 

#include "hw_def.h"


#ifndef QUEUE_H_
#define QUEUE_H_




#define MAX_QUEUE_SIZE		128

#define next(idx)			((idx + 1) & (MAX_QUEUE_SIZE - 1))

typedef struct
{
	uint8_t buf[MAX_QUEUE_SIZE];
	volatile uint8_t front;
	volatile uint8_t rear;
} circular_queue_t;

void queue_init(circular_queue_t *cq);
bool queue_is_full(circular_queue_t *cq);
bool queue_is_empty(circular_queue_t *cq);
bool enqueue(circular_queue_t *cq, uint8_t data);
bool dequeue(circular_queue_t *cq, uint8_t *data_out);

uint8_t queue_size(circular_queue_t *cq);
uint8_t queue_capacity(void);
bool queue_peek(circular_queue_t *cq, uint8_t *data_out);
bool queue_is_almost_full(circular_queue_t *cq, uint8_t threshold);

#endif /* QUEUE_H_ */