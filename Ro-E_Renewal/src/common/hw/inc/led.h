/*
 * led.h
 *
 * Created: 2025-04-24 오후 4:42:58
 *  Author: RCY
 */ 


#ifndef LED_H_
#define LED_H_

#include "hw_def.h"

typedef enum
{
	LED_RED,
	LED_GREEN,
	LED_BLUE,
	LED_ALL
}led_channel_t;

typedef struct 
{
	volatile uint8_t *port;
	volatile uint8_t *ddr;
	uint8_t red_mask;
	uint8_t green_mask;
	uint8_t blue_mask;
}rgb_led_t;

void led_init(void/*const rgb_led_t *led*/);
void led_on(unsigned char ch);
void led_off(unsigned char ch);


#endif /* LED_H_ */