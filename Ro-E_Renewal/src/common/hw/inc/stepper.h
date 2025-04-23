/*
 * stepper.h
 *
 * Created: 2025-04-23 오전 9:48:30
 *  Author: RCY
 */ 


#ifndef STEPPER_H_
#define STEPPER_H_


#include "hw_def.h"

#ifdef _USE_HW_STEP

unsigned char stepper_init(void);

#endif


#endif /* STEPPER_H_ */