/*
 * bsp.c
 *
 * Created: 2025-04-22 오후 6:27:29
 *  Author: RCY
 */ 

#include "bsp.h"
#include "timer.h"


void bsp_init(void)
{
	
}


uint32_t millis(void)//using cnt in timer0
{
	uint32_t m;
	uint8_t old_sreg = SREG;
	
	cli();
	m = timer0_ovf_cnt;
	SREG = old_sreg;
	
	return m;
}

unsigned long micros(void)//using cnt in timer1
{
	uint16_t t1;
	unsigned long ovf;

	cli(); // 인터럽트 중단
	t1 = TCNT1;
	ovf = timer1_ovf_cnt;
	if ((TIFR1 & (1 << TOV1)) && (t1 < 65535))
	{
		ovf++; // 오버플로우 직전 상황 대비
	}
	sei(); // 다시 허용

	// tick 단위로 계산 (1 tick = 0.5us)
	// 총 tick 수 = ovf * 65536 + t1
	// 총 us = tick * 0.5
	return ((ovf * 65536UL + t1) >> 1); // 나누기 2
}

void delay_ms(uint32_t ms)
{
	uint32_t target_ms = millis();
	while((millis() - target_ms) < ms);
}

void delay_us(uint32_t us)
{
	uint32_t target_us = micros();
	while((micros() - target_us) < us);
}
