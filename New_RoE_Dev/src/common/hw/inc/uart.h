/*
 * uart.h
 *
 * Created: 2025-05-09 오후 12:11:11
 *  Author: RCY
 */ 


#ifndef UART_H_
#define UART_H_

#include "hw_def.h"




#ifdef _USE_HW_UART

#define BAUD_9600			9600
#define BAUD_115200			115200

/* uart api */
void uart_init(uint32_t baud);
bool uart_send_byte_it(uint8_t data);
uint16_t uart_send_string_it(const char *str);
void uart_set_rx_callback(void (*callback)(uint8_t));
bool uart_tx_buffer_empty(void);
bool uart_data_available(void);
uint8_t uart_receive_byte(void);


#endif


#endif /* UART_H_ */