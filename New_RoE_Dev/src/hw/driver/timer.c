/*
 * timer.c
 *
 * Created: 2025-04-29 오후 3:56:55
 *  Author: RCY
 */ 

#include "uart.h"
#include "timer.h"

volatile uint32_t timer0_ovf_cnt;
volatile uint32_t timer1_ovf_cnt;


ISR(TIMER0_OVF_vect)//1ms interrupt
{
	TCNT0 = 6;
	timer0_ovf_cnt++;
}

//ISR(TIMER1_OVF_vect)//1us interrupt
//{
	//timer1_ovf_cnt++;
//}

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
	//TCCR1A = 0x00;			//normal mode
	//TCCR1B = (1 << CS11);	//presacler = 8
	//TIMSK1 = (1 << TOIE1);	//Overflow Enable
	//TCNT1 = 65534;			//1us = (8 * (TOP + 1 - TCNT1)) / 16,000,000
	//
	//sei();
}

/*1ms irq*/
void timer2_init(void)
{
	//TCCR2A = 0x00;//normal mode
	//TCCR2B = (1 << CS22);	//prescaler = 64
	//TIMSK0 = (1 << TOIE2);
	//TCNT2 = 6;
	//
	//sei();
}

static uint16_t _get_prescaler_bits(timer_id_t t, uint32_t *prescaler_out)
{
	*prescaler_out = 64;
	
	switch(t)
	{
		case TIMER1_ID : 
			return (1 << CS11) | (1 << CS10);
		case TIMER2_ID : 
			return (1 << CS22);
		default:
			return 0;
	}
}

void timer_pwm_init_channel(timer_id_t timer, pwm_ch_t ch/*, uint32_t freq_hz*/)
{
	uint32_t prescale;
	uint16_t cs_bits = _get_prescaler_bits(timer, &prescale);
	
	if(timer == TIMER1_ID)
	{
		/*Fast PWM, 8 bit mode - TOP Value(0x00FF)*/
		TCCR1A |= (1 << WGM10);
		TCCR1B |= (1 << WGM12);
		
		/*Compare Output - Clear OC1A/OC1B on compare match*/
		if(ch == PWM_CH_A)	TCCR1A |= (1 << COM1A1);
		else				TCCR1A |= (1 << COM1B1);
		
		TCCR1B |= cs_bits;//prescaler = 64
		
		if(ch == PWM_CH_A)	DDRB |= (1 << PORTB1);
		else				DDRB |= (1 << PORTB2);
		
		if(ch == PWM_CH_A)	OCR1A = 255;//RED_LED(pull-up resistor)
		else                OCR1B = 255;//GREEN_LED(pull-up resistor)
	}
	else if(timer == TIMER2_ID)
	{
		TCCR2A = (1 << WGM20) | (1 << WGM21);
		
		TCCR2A |= (1 << COM2A1);
		
		TCCR2B = cs_bits;
		
		DDRB |= (1 << PORTB3);
		OCR2A = 255;//BLUE_LED(pull-up resistor)
	}
}

void timer_pwm_set_duty(timer_id_t timer, pwm_ch_t ch, uint32_t duty)
{
	if(timer == TIMER1_ID)
	{
		if     (ch == PWM_CH_A)		OCR1A = (uint8_t)duty;
		else if(ch == PWM_CH_B)		OCR1B = (uint8_t)duty;
	}
	else if(timer == TIMER2_ID)
	{
		OCR2A = (uint8_t)duty;
	}
}

