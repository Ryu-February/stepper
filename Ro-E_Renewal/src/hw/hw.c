/*
 * hw.c
 *
 * Created: 2025-04-22 오후 6:18:03
 *  Author: RCY
 */ 

#include "hw.h"



void hw_init(void)
{
	bsp_init();
	
	sm_init_all();//step motor init
}