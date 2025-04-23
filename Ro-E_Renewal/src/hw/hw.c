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
	
	step_motor_left.init(&step_motor_left);
	step_motor_right.init(&step_motor_right);
}