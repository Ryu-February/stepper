/*
 * ap.c
 *
 * Created: 2025-04-22 오후 6:21:18
 *  Author: RCY
 */ 

#include "ap.h"

#define PWM_DUTY_ON		3
#define PWM_PERIOD		10


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
			led_set_color(RGB_GREEN);	
		}
		else
		{
			led_set_color(RGB_OFF);
			if(pwm_duty == PWM_PERIOD)
			{
				pwm_duty = 0;
			}
		}

		//roe_sm_operate();
		roe_operate_rogic(LEFT , 100, FORWARD);
		roe_operate_rogic(RIGHT, 100, FORWARD);
		
		//ms_operate(&ms_left, 30, FORWARD);
	}
}