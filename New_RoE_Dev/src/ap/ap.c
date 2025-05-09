/*
 * ap.c
 *
 * Created: 2025-04-22 오후 6:21:18
 *  Author: RCY
 */ 

#include "ap.h"

#define PWM_DUTY_ON		6
#define PWM_PERIOD		10

volatile uint8_t rgb_set_param = RGB_GREEN;

void switch_handler(bool pressed)
{
	if(pressed)
	{
		uint8_t reg_value = color_read16(TCS34725_DEVICE_ID);
		if(reg_value == 0x44)
		{
			rgb_set_param = RGB_WHITE;
		}
		else
		{
			rgb_set_param = RGB_RED;	
		}
	}
	else
	{
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
		//unsigned char pwm_duty = millis() % 10;
		//
		//if(pwm_duty < PWM_DUTY_ON)
		//{
			//led_set_color(rgb_set_param);	
		//}
		//else
		//{
			//led_set_color(RGB_OFF);
			//if(pwm_duty == PWM_PERIOD)
			//{
				//pwm_duty = 0;
			//}
		//}

		//roe_operate_rogic(LEFT , 10, FORWARD);
		//roe_operate_rogic(RIGHT, 10, FORWARD);		
		//ms_operate(LEFT, 30, FORWARD);
		
		if(switch_state != SW_EVENT_NONE)
		{
			if(_sw_cb)
				_sw_cb(switch_state == SW_EVENT_PRESSED);	
		}
		switch_state = SW_EVENT_NONE;
		
		led_set_color(rgb_set_param);
	}
}