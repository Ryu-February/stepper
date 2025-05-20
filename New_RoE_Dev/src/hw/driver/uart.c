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

volatile bool rx_event = false;

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

static char to_hexa_char(uint8_t nibble)
{
	return (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);//10이 넘어가면 A 문자열을 보내야 해서
}

void uart_dec_to_hexa(uint8_t no)
{
	char high = to_hexa_char(no >> 4);	//상위 4비트
	char low  = to_hexa_char(no & 0x0F);//하위 4비트
	
	uart_send_string_it("0x");
	uart_send_byte_it(high);
	uart_send_byte_it(low);
	uart_send_string_it("(Hexa Value)\r\n");
}

void uart_send_integer(int32_t num)
{	
	char buf[12];           /* 부호 + 10자리 + 널터미널 */
	char *p = buf;

	/* 1) 부호 처리 */
	if (num < 0) 
	{
		*p++ = '-';
		num = -num;
	}

	/* 2) 숫자 자리 채우기 (역순) */
	char *start = p;
	int32_t tmp = num;
	do {
		*p++ = '0' + (tmp % 10);
		tmp /= 10;
	} while (tmp > 0);

	/* 3) 역순으로 넣었으니 뒤집기 */
	char *end = p - 1;
	while (start < end) 
	{
		char t = *start;
		*start++ = *end;
		*end-- = t;
	}

	/* 4) 문자열 끝 마무리 및 출력 */
	*p = '\0';
	uart_send_string_it(buf);
	//uart_send_string_it("\r\n");
}
