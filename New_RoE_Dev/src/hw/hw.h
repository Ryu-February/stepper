/*
 * hw.h
 *
 * Created: 2025-04-22 오후 6:23:42
 *  Author: RCY
 */ 


#ifndef HW_H_
#define HW_H_

#include "hw_def.h"

#include "timer.h"
#include "i2c.h"
#include "stepper.h"
#include "color.h"
#include "led.h"
#include "switch.h"
#include "uart.h"
#include "buzzer.h"

void hw_init(void);

#endif /* HW_H_ */