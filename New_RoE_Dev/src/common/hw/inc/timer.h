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

typedef enum
{
	TIMER0_ID = 0,
	TIMER1_ID,
	TIMER2_ID	
}timer_id_t;

typedef enum
{
	PWM_CH_A = 0,
	PWM_CH_B,
}pwm_ch_t;

extern volatile uint32_t timer0_ovf_cnt;
extern volatile uint32_t timer1_ovf_cnt;

void timer0_init(void);	//1ms	irq
void timer1_init(void);	//1us	irq
void timer2_init(void);	//1ms	irq
void timer_pwm_init_channel(timer_id_t timer, pwm_ch_t ch/*, uint32_t freq_hz*/);
void timer_pwm_set_duty(timer_id_t timer, pwm_ch_t ch, uint32_t duty);

#endif

#endif /* TIMER_H_ */