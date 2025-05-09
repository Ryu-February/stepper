/*
 * switch.c
 *
 * Created: 2025-05-09 오후 12:18:23
 *  Author: RCY
 */ 
#include "switch.h"

static const switch_t sw =
{
	&PORTD,
	&DDRD,
	SW_MASK
};

void switch_init(void)
{
	*(sw.ddr) &= ~(sw.sw_mask);
}
