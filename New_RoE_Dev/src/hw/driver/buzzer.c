/*
 * buzzer.c
 *
 * Created: 2025-05-21 오전 10:04:21
 *  Author: RCY
 */ 

#include "buzzer.h"


static const buzzer_t buz =
{
	&PORTD,
	&DDRD,
	BUZ_MASK,
};


void buzzer_init(void)
{
	*(buz.ddr) |= buz.mask;
	*(buz.port) &= ~buz.mask;
}

void buzzer_on(void)
{
	*(buz.port) |= buz.mask;
}

void buzzer_off(void)
{
	*(buz.port) &= ~(buz.mask);
}
