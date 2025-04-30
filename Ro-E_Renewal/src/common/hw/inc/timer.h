/*
 * timer.h
 *
 * Created: 2025-04-29 오후 3:57:09
 *  Author: RCY
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "hw_def.h"

#ifdef _USE_HW_TIM

#include "led.h"

void timer0_init(void);	//1ms	irq
void timer1_init(void);	//1us	irq
void timer2_init(void);	//100us	irq

#endif

#endif /* TIMER_H_ */