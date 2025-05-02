/*
 * bsp.h
 *
 * Created: 2025-04-22 오후 6:27:36
 *  Author: RCY
 */ 


#ifndef BSP_H_
#define BSP_H_

#include "def.h"

void bsp_init(void);

uint32_t millis(void);
uint32_t micros(void);

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif /* BSP_H_ */