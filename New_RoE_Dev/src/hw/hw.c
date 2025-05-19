/*
 * hw.c
 *
 * Created: 2025-04-22 오후 6:18:03
 *  Author: RCY
 */ 

#include "hw.h"



void hw_init(void)
{
	bsp_init();
	
	
	timer0_init();
	timer1_init();
	timer2_init();
	i2c_init();
	uart_init(115200);
	
	led_init();
	color_init();
	switch_init();
	
	roe_sm_init();//Ro-E step motor init
	
}