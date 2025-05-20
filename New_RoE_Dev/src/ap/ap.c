/*
 * ap.c
 *
 * Created: 2025-04-22 오후 6:21:18
 *  Author: RCY
 */ 

#include "ap.h"



volatile uint8_t rgb_set_param = RGB_GREEN;


extern volatile bool rx_event;

uint16_t *c, *r, *g, *b;

void switch_handler(bool pressed)
{
	if(pressed)
	{
		//color_read_rgbc(c, r, g, b);
		uart_send_string_it("reading...\r\n");
		rgb_set_param = RGB_WHITE;
	}
	else
	{
		uart_send_string_it("not reading...\r\n");
		rgb_set_param = RGB_MAGENTA;
	}
}

bool sw_get_event(void)
{
	if(switch_state)
	{
		return true;	
	}
	return false;
}

void ap_init(void)
{
	switch_attach_callback(switch_handler);
}

void ap_main(void)
{
	while(1)
	{
		if(switch_state != SW_EVENT_NONE)
		{
			if(_sw_cb)
				_sw_cb(switch_state == SW_EVENT_PRESSED);	
		}
		switch_state = SW_EVENT_NONE;
		
		led_set_color(rgb_set_param);
		
		color_read_rgbc(c, r, g, b);
		
		
		uart_send_string_it("color data | ");
		uart_send_string_it("c : ");
		uart_send_integer(*c);
		uart_send_string_it("|| r : ");
		uart_send_integer(*r);
		uart_send_string_it("|| g : ");
		uart_send_integer(*g);
		uart_send_string_it("|| b : ");
		uart_send_integer(*b);
		uart_send_string_it("\r\n");
		delay_ms(10);
	}
}