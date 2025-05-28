/*
 * ap.c
 *
 * Created: 2025-04-22 오후 6:21:18
 *  Author: RCY
 */ 

#include "led.h"
#include "ap.h"



volatile uint8_t rgb_set_param = CLR_BLACK;

uint16_t c_val, r_val, g_val, b_val;
uint16_t *c, *r, *g, *b;

unsigned char uart_update = false;
uint32_t cur_time = 0;

void switch_handler(bool pressed)
{
	if(!pressed)
	{
		return;
	}
	
	uart_send_string_it("switch pressed\r\n");
	
	if(!color_read_rgbc(c, r, g, b))
	{
		uart_send_string_it("integration time is not ready...\r\n");
		return;
	}
	
	uart_send_string_it("reading...\r\n");
	
	uart_send_string_it("color data -> ");
	uart_send_string_it("c : ");
	uart_send_integer(*c);
	uart_send_string_it(" || r : ");
	uart_send_integer(*r);
	uart_send_string_it(" || g : ");
	uart_send_integer(*g);
	uart_send_string_it(" || b : ");
	uart_send_integer(*b);
	uart_send_string_it("\r\n");
	
	uint16_t hue = color_calc_hue(c, r, g, b);
	
	uint8_t what_color = color_from_hue(hue);
	
	
	uart_send_string_it("hue value : ");
	uart_send_integer(hue);
	uart_send_string_it("\r\n");
	
	if(what_color == COLOR_RED)
	{
		rgb_set_param = CLR_RED;
		uart_send_string_it("current color is red\r\n");
	}
	else if(what_color == COLOR_GREEN)
	{
		rgb_set_param = CLR_GREEN;
		uart_send_string_it("current color is green\r\n");
	}
	else if(what_color == COLOR_BLUE)
	{
		rgb_set_param = CLR_BLUE;
		uart_send_string_it("current color is blue\r\n");
	}
	else
	{
		rgb_set_param = CLR_WHITE;
		uart_send_string_it("current color is unknown...\r\n");
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
	c = &c_val;
	r = &r_val;
	g = &g_val;
	b = &b_val;
	
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
	}
}