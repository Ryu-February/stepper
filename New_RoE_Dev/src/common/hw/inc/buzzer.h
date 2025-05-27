/*
 * buzzer.h
 *
 * Created: 2025-05-21 오전 10:04:29
 *  Author: RCY
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_


#include "hw_def.h"
#include "pitches.h"

#define BUZ_PIN		PORTD3

#define BUZ_MASK	(1 << BUZ_PIN)

#define BUZ_INTERVAL_MS   500   // 0.5초
#define BUZ_TOTAL_PHASES  6     // on/off 3번: 총 6단계

typedef struct
{
	volatile uint8_t	*port;
	volatile uint8_t	*ddr;
			 uint8_t	 mask;
	
}buzzer_t;

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);



#endif /* BUZZER_H_ */