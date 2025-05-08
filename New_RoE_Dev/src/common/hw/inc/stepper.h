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

	#define LEFT				0
	#define RIGHT				1
	#define FORWARD				0
	#define REVERSE				1
	
	//max slew speed : 1500Hz mininmum -> 1/1500 ~= 666us
	#define SAFE_TORQUE_FREQ	800		//unit : Hz
	#define MAX_PERIOD_US		1200	//unit : us -> 1.2ms
	#define MIN_PERIOD_US		100000	//unit : us -> 100ms
	
	#define MAX_RPM				2250
	#define SAFE_MAX_RPM		1200
	#define MIN_RPM				0
	#define MIN_SPEED			0
	#define MAX_SPEED			100
	
	#define MICRO_MAX_PWM		255
	
	#if (_USE_STEP_MODE == _STEP_MODE_HALF)
		#define STEP_MASK 0x07
		#define STEP_PER_REV 40
	#elif (_USE_STEP_MODE == _STEP_MODE_FULL)
		#define STEP_MASK 0x03
		#define STEP_PER_REV 20
	#elif (_USE_STEP_MODE == _STEP_MODE_MICRO)
		#define STEP_MASK 0x0F
		#define STEP_PER_REV 80
	#endif
	
	// 하프스텝(8단계) 시퀀스용 테이블 -> 9° (18 % 2)
	// { AIN1, AIN2, BIN1, BIN2 }
	#if (_USE_STEP_MODE == _STEP_MODE_HALF)
	static const uint8_t STEP_TABLE[8][4] = 
	{
		{1,0,1,0}, 
		{0,0,1,0}, 
		{0,1,1,0}, 
		{0,1,0,0},
		{0,1,0,1}, 
		{0,0,0,1}, 
		{1,0,0,1}, 
		{1,0,0,0},
	};

	// 풀스탭(4단계) 시퀀스용 테이블 -> 18°(step angle)
	#elif (_USE_STEP_MODE == _STEP_MODE_FULL)
	static const uint8_t STEP_TABLE[4][4] = 
	{
		{1,0,1,0},  // A+ & B+
		{0,1,1,0},  // A- & B+
		{0,1,0,1},  // A- & B-
		{1,0,0,1},  // A+ & B-
	};
	#elif (_USE_STEP_MODE == _STEP_MODE_MICRO)
	static const uint8_t STEP_TABLE[16][2] =
	{
		{128, 255}, {180, 240},
		{224, 200}, {253, 144},
		{255,  80}, {238,  28},
		{200,   2}, {144,   0},
		{ 80,   4}, { 28,  28},
		{  2,  80}, {  0, 144},
		{  4, 200}, { 28, 240},
		{ 80, 255}, {144, 253}
	};
	#endif
	
	// 1) StepMotor 구조체 선언
	typedef struct StepMotor 
	{
		volatile uint8_t *ain1_port, *ain1_ddr; uint8_t ain1_pin;
		volatile uint8_t *ain2_port, *ain2_ddr; uint8_t ain2_pin;
		volatile uint8_t *bin1_port, *bin1_ddr; uint8_t bin1_pin;
		volatile uint8_t *bin2_port, *bin2_ddr; uint8_t bin2_pin;
	
		uint8_t step_idx;
		
		uint32_t period_us;
		uint32_t prev_time;
	
		void (*init)(struct StepMotor*);
		void (*slide)(struct StepMotor*);
		void (*forward)(struct StepMotor*);
		void (*reverse)(struct StepMotor*);
		void (*brake)(struct StepMotor*);
	} StepMotor;
	
	typedef struct
	{
		StepMotor *base;
		uint8_t micro_idx;
	}MicroStepMotor;
	
	extern MicroStepMotor ms_left, ms_right;
	
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
	void roe_sm_reverse(void);
	void roe_sm_brake(void);
	
	
	uint32_t pwm_to_rpm(uint8_t speed_pwm);
	uint32_t rpm_to_period(uint16_t rpm);
	
	void roe_sm_operate(void);
	void roe_operate_rogic(uint8_t m_pin, uint8_t speed, unsigned char m_dir);
	
	//void ms_operate(MicroStepMotor *m, uint8_t speed, uint8_t m_dir);
	void ms_operate(uint8_t m_pin, uint8_t speed, uint8_t m_dir);
#endif


#endif /* STEPPER_H_ */