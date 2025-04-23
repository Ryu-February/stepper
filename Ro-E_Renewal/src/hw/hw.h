/*
 * hw.h
 *
 * Created: 2025-04-22 오후 6:23:42
 *  Author: RCY
 */ 


#ifndef HW_H_
#define HW_H_

#include "hw_def.h"

#include "stepper.h"

#ifdef _USE_HW_STEP

extern StepMotor step_motor_left;
extern StepMotor step_motor_right;

#endif

void hw_init(void);

#endif /* HW_H_ */