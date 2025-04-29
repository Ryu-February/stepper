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
	
	led_init();
	roe_sm_init();//Ro-E step motor init
}