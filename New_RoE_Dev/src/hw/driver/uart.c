/*
 * uart.c
 *
 * Created: 2025-05-09 오후 12:10:58
 *  Author: RCY
 */ 

#include "led.h"
#include "queue.h"
#include "uart.h"



static circular_queue_t tx_cq;
static circular_queue_t rx_cq;
static void (*rx_callback)(uint8_t) = 0;

volatile bool rx_event = false;

void uart_init(uint32_t baud) 
{
	uint16_t ubrr = (F_CPU / (8UL * baud)) - 1;
	/* set baud rate */
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	
	/*Enable double speed*/
	UCSR0A |= (1 << U2X0);
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
	bool enqueued = enqueue(&tx_cq, data);
	if (enqueued) 
	{
		/* enable udre interrupt to start transmission */
		UCSR0B |= (1 << UDRIE0);
	}
	
	return enqueued;
}

bool uart_send_string_it(const char *str) 
{
	while (*str) 
	{
		if (!uart_send_byte_it((uint8_t)*str++))
		{
			return false;
		}
	}
	return true;
}

bool uart_read_byte(uint8_t *data_out)
{
	return dequeue(&rx_cq, data_out);
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
	uint8_t data_out = 0;
	while (!dequeue(&rx_cq, &data_out));
	return data_out;
}

ISR(USART0_UDRE_vect) 
{
	uint8_t data_out;
	if (dequeue(&tx_cq, &data_out)) 
	{
		UDR0 = data_out;
	} 
	else 
	{
		/* buffer empty, disable udre interrupt */
		UCSR0B &= ~(1 << UDRIE0);
	}
}

//ISR(USART0_RX_vect) 
//{
	//uint8_t received_data = UDR0;
	//
	//enqueue(&rx_cq, received_data);
	//rx_event = true;
//}


void uart_dec_to_hexa(uint8_t no)
{
	uint8_t hexa[2] = {0};
	uint8_t tempValue_Tens, tempValue_Ones = 0;
	
	tempValue_Tens = no / 16;
	tempValue_Ones = no % 16;
	
	if(tempValue_Tens >= 10)
	{
		hexa[0] = tempValue_Tens + 0x37;		//10진수를 대문자로 변환해주려고 0x37을 더함
	}
	else if(tempValue_Tens < 10)
	{
		hexa[0] = tempValue_Tens + 0x30;		//10진수를 숫자 문자열로 변환
	}
	
	if(tempValue_Ones >= 10)
	{
		hexa[1] = tempValue_Ones + 0x37;
	}
	else if(tempValue_Ones < 10)
	{
		hexa[1] = tempValue_Ones + 0x30;		//10진수를 숫자 문자열로 변환
	}
	
	/*TX_String3("Hexa value : 0x");*/
	uart_send_string_it("0x");
	uart_send_byte_it(hexa[0]);
	uart_send_byte_it(hexa[1]);
	uart_send_string_it("(Hexa Value)\r\n");
}
