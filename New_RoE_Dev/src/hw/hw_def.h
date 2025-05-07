/*
 * hw_def.h
 *
 * Created: 2025-04-22 오후 6:24:02
 *  Author: RCY
 */ 


#ifndef HW_DEF_H_
#define HW_DEF_H_

#include "def.h"
#include "bsp.h"

#define _USE_HW_STEP			//STEP_MOTOR
// Select one stepping mode by setting _USE_STEP_MODE:
	typedef enum
	{
		_STEP_MODE_HALF	 = 1,
		_STEP_MODE_FULL  = 2,
		_STEP_MODE_MICRO = 3
	}StepMode_t;

	#define _USE_STEP_MODE _STEP_MODE_MICRO

#define _USE_HW_LED				//LED
#define _USE_HW_TIM				//TIMER

#endif /* HW_DEF_H_ */