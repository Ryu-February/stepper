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

// 하프스텝(8단계) 시퀀스용 테이블
// { AIN1, AIN2, BIN1, BIN2 }
#if _USE_STEP_HALF
static const uint8_t STEP_TABLE[8][4] = {
	{1,0,1,0}, 
	{0,0,1,0}, 
	{0,1,1,0}, 
	{0,1,0,0},
	{0,1,0,1}, 
	{0,0,0,1}, 
	{1,0,0,1}, 
	{1,0,0,0},
};

// 풀스탭(4단계) 시퀀스용 테이블
#else //_USE_STEP_FULL
static const uint8_t STEP_TABLE[4][4] = {
	{1,0,1,0},  // A+ & B+
	{0,1,1,0},  // A- & B+
	{0,1,0,1},  // A- & B-
	{1,0,0,1},  // A+ & B-
};
#endif

// 1) StepMotor 구조체 선언
typedef struct StepMotor {
	volatile uint8_t *ain1_port, *ain1_ddr;//구조체 멤버
	uint8_t           ain1_pin;
	volatile uint8_t *ain2_port, *ain2_ddr;
	uint8_t           ain2_pin;
	volatile uint8_t *bin1_port, *bin1_ddr;
	uint8_t           bin1_pin;
	volatile uint8_t *bin2_port, *bin2_ddr;
	uint8_t           bin2_pin;

	uint8_t step_idx;
	
	void (*init)(struct StepMotor*);
	void (*slide)(struct StepMotor*);
	void (*forward)(struct StepMotor*);
	void (*reverse)(struct StepMotor*);
	void (*brake)(struct StepMotor*);
} StepMotor;

// 2) 함수 원형
void sm_init   (StepMotor *m);
void sm_slide  (StepMotor *m);
void sm_forward(StepMotor *m);
void sm_reverse(StepMotor *m);
void sm_brake  (StepMotor *m);

// 3) DEFINE_STEP_MOTOR 매크로 (꼭 구조체 정의 다음에!)
#define DEFINE_STEP_MOTOR(name,			\
a1p, a1d, a1,    \
a2p, a2d, a2,    \
b1p, b1d, b1,    \
b2p, b2d, b2)    \
StepMotor name = \
{									   \
	/* AIN1 */                         \
	.ain1_port = &a1p,                 \
	.ain1_ddr  = &a1d,                 \
	.ain1_pin  = a1,                   \
	/* AIN2 */                         \
	.ain2_port = &a2p,                 \
	.ain2_ddr  = &a2d,                 \
	.ain2_pin  = a2,                   \
	/* BIN1 */                         \
	.bin1_port = &b1p,                 \
	.bin1_ddr  = &b1d,                 \
	.bin1_pin  = b1,                   \
	/* BIN2 */                         \
	.bin2_port = &b2p,                 \
	.bin2_ddr  = &b2d,                 \
	.bin2_pin  = b2,                   \
	/*step_index*/					   \
	.step_idx  = 0,					   \
	/* 함수 포인터 */                   \
	.init    = sm_init,                \
	.slide   = sm_slide,               \
	.forward = sm_forward,             \
	.reverse = sm_reverse,             \
	.brake   = sm_brake                \
};

void roe_sm_init(void);
void roe_sm_forward(void);
void roe_sm_backward(void);
void roe_sm_operate(void);

#endif


#endif /* STEPPER_H_ */