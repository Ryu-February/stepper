/*
 * timer.c
 *
 * Created: 2025-04-29 오후 3:56:55
 *  Author: RCY
 */ 
#include "timer.h"

volatile uint32_t timer0_ovf_cnt;
volatile uint32_t timer1_ovf_cnt;

ISR(TIMER0_OVF_vect)//1ms interrupt
{
	TCNT0 = 6;
	timer0_ovf_cnt++;
}

ISR(TIMER1_OVF_vect)//1us interrupt
{
	timer1_ovf_cnt++;
}

/*1ms irq*/
void timer0_init(void)						//Interrupt per 1ms
{
	TCCR0A = 0x00;
	TCCR0B = (1 << CS00) | (1 << CS01);		//prescaler = 64
	TIMSK0 = (1 << TOIE0);
	TCNT0 = 6;								//1ms = (64 * (TOP + 1 - TCNT0)) / 16,000,000
	
	sei();
}

/*1us irq*/
void timer1_init(void)
{
	TCCR1A = 0x00;			//normal mode
	TCCR1B = (1 << CS11);	//presacler = 8
	TIMSK1 = (1 << TOIE1);	//Overflow Enable
	TCNT1 = 65534;			//1us = (8 * (TOP + 1 - TCNT1)) / 16,000,000
	
	sei();
}

/*100us irq*/
void timer2_init(void)
{
	TCCR2A = 0x00;//normal mode
	TCCR2B = (1 << CS22);
	TIMSK0 = (1 << TOIE2);
	TCNT2 = 231;
	
	sei();
}


