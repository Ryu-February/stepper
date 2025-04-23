/*
 * stepper.c
 *
 * Created: 2025-04-22 오후 6:33:18
 *  Author: RCY
 */ 

#include "stepper.h"

//motor driver(PN7713)
//pc0,1,2,3		-> motor_left
//pb4,5 | pc4,5 -> motor_right

unsigned char stepper_init(void)
{
	bool ret = true;
	
	/*left motor*/
	DDRC  = (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);
	
	/*right motor*/
	DDRC |= (1 << PC4) | (1 << PC5);
	DDRB  = (1 << PB4) | (1 << PB5);
	
	return ret;
}