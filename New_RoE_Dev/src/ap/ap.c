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

void ap_init(void)
{
	
}

void ap_main(void)
{
	while(1)
	{
		unsigned char pwm_duty = millis() % 10;
		
		if(pwm_duty < PWM_DUTY_ON)
		{
			led_set_color(rgb_set_param);	
		}
		else
		{
			led_set_color(RGB_OFF);
			if(pwm_duty == PWM_PERIOD)
			{
				pwm_duty = 0;
			}
		}

		//roe_operate_rogic(LEFT , 10, FORWARD);
		//roe_operate_rogic(RIGHT, 10, FORWARD);		
		//ms_operate(LEFT, 30, FORWARD);
		
		//if(color_read16(TCS34725_DEVICE_ID) == 0x44)
		//{
			//rgb_set_param = RGB_WHITE;
		//}
		
		if(switch_check())
		{
			rgb_set_param = RGB_MAGENTA;
		}
		else
		{
			rgb_set_param = RGB_WHITE;
		}
		//delay_ms(20);
	}
}