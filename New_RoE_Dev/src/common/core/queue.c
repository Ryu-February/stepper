/*
 * queue.c
 *
 * Created: 2025-05-16 오후 5:58:08
 *  Author: RCY
 */ 

#include "queue.h"


/**
 * @brief initialize a circular buffer
 * @param cq pointer to buffer
 */
void queue_init(circular_queue_t *cq) 
{
    cq->front = 0;
    cq->rear = 0;
}

/**
 * @brief check if buffer is full
 * @param cq pointer to buffer
 * @return true if full`
 */
bool queue_is_full(circular_queue_t *cq) 
{
    return (next(cq->rear) == cq->front);
}

/**
 * @brief check if buffer is empty
 * @param cq pointer to buffer
 * @return true if empty
 */
bool queue_is_empty(circular_queue_t *cq) 
{
    return (cq->front == cq->rear);
}

/**
 * @brief push a byte into buffer (if not full)
 * @param cq pointer to buffer
 * @param data byte to push
 * @return true if pushed, false if full
 */
bool enqueue(circular_queue_t *cq, uint8_t data) 
{
    if (queue_is_full(cq))
	{
		return false;
	}
	
    cq->buf[cq->rear] = data;
	cq->rear = next(cq->rear);
    
	return true;
}

/**
 * @brief pop a byte from buffer (if not empty)
 * @param cq pointer to buffer
 * @param data pointer to output byte
 * @return true if popped, false if empty
 */
bool dequeue(circular_queue_t *cq, uint8_t *data_out) 
{
    if (queue_is_empty(cq))
	{
		return false;
	}
	
	*data_out = cq->buf[cq->front];
    cq->front = next(cq->front);
	
    return true;
}

void queue_clear(circular_queue_t *cq)
{
	cq->front = cq->rear;
}


/**
 * @brief Get the current number of elements in the queue
 * @param cq Pointer to the queue
 * @return Number of elements currently stored
 */
uint8_t queue_size(circular_queue_t *cq)
{
	return (cq->rear - cq->front + MAX_QUEUE_SIZE) & (MAX_QUEUE_SIZE - 1);
}

/**
 * @brief Get the max capacity of the queue
 * @return Max number of elements the queue can hold
 */
uint8_t queue_capacity(void)
{
	return (MAX_QUEUE_SIZE - 1);
}


/**
 * @brief Peek at the next byte to be dequeued without removing it
 * @param cq Pointer to the queue
 * @param data_out Pointer to store peeked byte
 * @return true if peeked successfully, false if queue is empty or data_out is NULL
 */
bool queue_peek(circular_queue_t *cq, uint8_t *data_out)
{
	if(queue_is_empty(cq) || data_out == NULL)
	{
		return false;
	}
	
	*data_out = cq->buf[cq->front];
	return true;
}

/**
 * @brief Check if the queue is almost full
 * @param cq Pointer to the queue
 * @param threshold Number of free slots threshold
 * @return true if free slots <= threshold, false otherwise
 */
bool queue_is_almost_full(circular_queue_t *cq, uint8_t threshold)
{
	uint8_t free_space = queue_capacity() - queue_size(cq);
	return (free_space <= threshold);
}