/*
 * uart.c
 *
 * Created: 2025-05-09 오후 12:10:58
 *  Author: RCY
 */ 

#include "queue.h"
#include "uart.h"



static circular_queue_t tx_cq;
static circular_queue_t rx_cq;
static void (*rx_callback)(uint8_t) = 0;

void uart_init(uint32_t baud) 
{
	uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;
	/* set baud rate */
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	/* enable receiver, transmitter and rx complete interrupt */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	/* set frame format: 8data, 1 stop bit, no parity */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	/* initialize buffers */
	queue_init(&tx_cq);
	queue_init(&rx_cq);
	/* enable global interrupts */
	sei();
}

bool uart_send_byte_it(uint8_t data) 
{
	bool ok = enqueue(&tx_cq, data);
	if (ok) 
	{
		/* enable udre interrupt to start transmission */
		UCSR0B |= (1 << UDRIE0);
	}
	return ok;
}

uint16_t uart_send_string_it(const char *str) 
{
	uint16_t count = 0;
	while (*str) 
	{
		if (!uart_send_byte_it((uint8_t)*str))
		{
			break;
		}
		str++;
		count++;
	}
	return count;
}

void uart_set_rx_callback(void (*callback)(uint8_t)) 
{
	rx_callback = callback;
}

bool uart_tx_buffer_empty(void) 
{
	return queue_is_empty(&tx_cq);
}

bool uart_data_available(void) 
{
	return !queue_is_empty(&rx_cq);
}

uint8_t uart_receive_byte(void) 
{
	uint8_t *data_out = 0;
	while (!dequeue(&rx_cq, data_out));
	return *data_out;
}

ISR(USART0_UDRE_vect) 
{
	uint8_t *data_out = 0;
	if (dequeue(&tx_cq, data_out)) 
	{
		UDR0 = *data_out;
	} 
	else 
	{
		/* buffer empty, disable udre interrupt */
		UCSR0B &= ~(1 << UDRIE0);
	}
}

ISR(USART0_RX_vect) {
	uint8_t data = UDR0;
	if (enqueue(&rx_cq, data)) 
	{
		/* optionally notify via callback */
		if (rx_callback) rx_callback(data);
	}
}
