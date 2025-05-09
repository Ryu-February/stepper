/*
 * switch.c
 *
 * Created: 2025-05-09 오후 12:18:23
 *  Author: RCY
 */ 
#include "switch.h"
#include "led.h"

static switch_cb_t _sw_cb = NULL;

static const switch_t sw =
{
	&PIND,
	&DDRD,
	SW_MASK
};

void switch_init(void)
{
	*(sw.ddr) &= ~(sw.sw_mask);
	
	PCICR  |= (1 << PCIE2);
	PCMSK2 |= sw.sw_mask;//PD2 -> PCINT18
	
	sei();
}

bool switch_check(void)
{
	#if (_USE_SW_MODE == _SW_MODE_POL)
	
	if((*sw.pin & sw.sw_mask) == 0)//pull-up sw
	{
		return true;
	}
	return false;
	
	//#elif (_USE_SW_MODE == _SW_MODE_IRQ)
	//
	//return sw_pressed;
	
	#else
	
	return false;
	
	#endif
}

void switch_attach_callback(switch_cb_t cb)
{
	_sw_cb = cb;
}

void switch_handler(bool pressed)
{
	if(pressed)
	{
		led_set_color(RGB_MAGENTA);
	}
	else
	{
		led_set_color(RGB_WHITE);
	}
}

ISR(PCINT2_vect)
{
	static uint8_t prev_state = 1;
	uint8_t current_state = ((*sw.pin & sw.sw_mask) ? 1 : 0);
	
	if(prev_state != current_state && _sw_cb)//콜백 함수가 연동됐는지 확인하기 위함
	{
		_sw_cb(current_state == 0);
	}
	prev_state = current_state;
}