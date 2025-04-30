/*
 * ap.c
 *
 * Created: 2025-04-22 오후 6:21:18
 *  Author: RCY
 */ 

#include "ap.h"

extern volatile uint32_t timer0_ovf_cnt;

void ap_init(void)
{
	
}

void ap_main(void)
{
	while(1)
	{
		led_set_color(RGB_WHITE);
		//delay_ms(500);
		delay_us(1000);
		led_set_color(RGB_OFF);
		//delay_ms(500);
		delay_us(1000);
		//roe_sm_operate();
	}
}