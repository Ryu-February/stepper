/*
 * led.c
 *
 * Created: 2025-04-24 오후 4:42:49
 *  Author: RCY
 */ 

#include "led.h"

//RGB_LED
//R LED -> PB1
//G LED -> PB2
//B LED -> PB3

void led_init(void/*const rgb_led_t *led*/)
{
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
	//*(led->ddr) |= (led->red_mask) | (led->green_mask) | (led->blue_mask);
}

void led_all_init(void)
{
	//led_init(led->)
}

void led_on(unsigned char ch)
{
	switch (ch)
	{
		case 1 : 
		{
			PORTB &= (1 << PB1);
			PORTB |= (1 << PB2) | (1 << PB3);
		}
		
		case 2 :
		{
			PORTB &= (1 << PB1);
			PORTB |= (1 << PB2) | (1 << PB3);
		}
		
		case 3 :
		{
			PORTB &= (1 << PB1);
			PORTB |= (1 << PB2) | (1 << PB3);
		}
	}
}

void led_off(unsigned char ch)
{
	if(ch == 1)
	{
		PORTB &= ~(1 << PB1);
	}
}