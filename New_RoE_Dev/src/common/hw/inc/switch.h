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

#define SW_PIN	PIND2		//Read switch state from PIN register(PIND2), not PORTD2

#define SW_MASK	(1 << SW_PIN)

typedef struct  
{
	volatile uint8_t *pin; //Read switch state from PIN register(PIND2), not PORTD2
	volatile uint8_t *ddr;
	uint8_t sw_mask;
}switch_t;

void switch_init(void);
bool switch_check(void);

#endif

#endif /* SWITCH_H_ */