/*
 * switch.c
 *
 * Created: 2025-05-09 오후 12:18:23
 *  Author: RCY
 */ 
#include "switch.h"

static volatile bool sw_pressed = false;
static void (*_sw_cb)(void) = 0;

static const switch_t sw =
{
	&PIND,
	&DDRD,
	SW_MASK
};

void switch_init(void)
{
	*(sw.ddr) &= ~(sw.sw_mask);
	
	//PCICR  |= (1 << PCIE2);
	//PCMSK2 |= (1 << SW_PIN);
	//
	//sei();
}

bool switch_check(void)
{
	if((*sw.pin & sw.sw_mask) == 0)//pull-up sw
	{
		return true;
	}
	return false;
}

//ISR(INT0_vect)
//{
	//if((*sw.port & sw.sw_mask) == 0)
	//{
		//sw_pressed = true;
		//if(_sw_cb)
		//{
			//_sw_cb();
		//}
	//}
//}