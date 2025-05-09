/*
 * switch.h
 *
 * Created: 2025-05-09 오후 12:18:31
 *  Author: RCY
 */ 


#ifndef SWITCH_H_
#define SWITCH_H_

#include "hw_def.h"

#ifdef _USE_HW_SW

#define SW_PIN	PORTD2

#define SW_MASK	(1 << SW_PIN)

typedef struct  
{
	volatile uint8_t *port;
	volatile uint8_t *ddr;
	uint8_t sw_mask;
}switch_t;

#endif

#endif /* SWITCH_H_ */