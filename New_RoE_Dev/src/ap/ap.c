/*
 * ap.c
 *
 * Created: 2025-04-22 오후 6:21:18
 *  Author: RCY
 */ 

#include "ap.h"



volatile uint8_t rgb_set_param = RGB_GREEN;

volatile uint8_t reg_value;

extern volatile bool rx_event;

void switch_handler(bool pressed)
{
	if(pressed)
	{
		reg_value = color_read16(TCS34725_ID_REG);
	}
	else
	{
		reg_value = 0;
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

		
		if(reg_value == TCS34725_DEVICE_ID)
		{
			rgb_set_param = RGB_WHITE;
		}
		else
		{
			rgb_set_param = RGB_MAGENTA;
		}
		
		led_set_color(rgb_set_param);
		
		uart_send_string_it("device ID: ");
		uart_dec_to_hexa(reg_value);
		uart_send_string_it("\r\n");
		delay_ms(10);
	}
}