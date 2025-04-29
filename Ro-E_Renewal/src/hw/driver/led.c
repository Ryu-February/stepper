/*
 * led.c
 *
 * Created: 2025-04-24 오후 4:42:49
 *  Author: RCY
 */ 

#include "led.h"

static const rgb_led_t led = 
{
	&PORTB,
	&DDRB,
	LED_RED_MASK,
	LED_GREEN_MASK,
	LED_BLUE_MASK
};

//RGB_LED
//R LED -> PB1
//G LED -> PB2
//B LED -> PB3

void led_init(void)
{
	*(led.ddr) |= (led.red_mask) | (led.green_mask) | (led.blue_mask);
	*(led.port) |= (led.red_mask) | (led.green_mask) | (led.blue_mask);//led->pullup registor
}

void led_set_color(uint8_t rgb_color)
{
	if(rgb_color & RGB_COLOR_RED)
	{
		*(led.port) &= ~led.red_mask;
	}
	else
	{
		*(led.port) |= led.red_mask;
	}
	
	if(rgb_color & RGB_COLOR_GREEN)
	{
		*(led.port) &= ~led.green_mask;
	}
	else
	{
		*(led.port) |= led.green_mask;
	}
	
	if(rgb_color & RGB_COLOR_BLUE)
	{
		*(led.port) &= ~led.blue_mask;
	}
	else
	{
		*(led.port) |= led.blue_mask;
	}
}