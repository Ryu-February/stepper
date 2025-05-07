/*
 * stepper.c
 *
 * Created: 2025-04-22 오후 6:33:18
 *  Author: RCY
 */ 

#include "stepper.h"

/*                            Motor(15BY25-119)                         */
/*                          Motor driver(PN7713)                        */

/************************************************************************/
/*                       MCU  |     NET    | DRIVER                     */
/*                       PC1 -> MOT_L_IN1 -> AIN1                       */
/*                       PC0 -> MOT_L_IN2 -> AIN2                       */
/*                       PC3 -> MOT_L_IN3 -> BIN1                       */
/*                       PC2 -> MOT_L_IN4 -> BIN2                       */
/*                                                                      */
/*                       PC5 -> MOT_R_IN1 -> AIN1                       */
/*                       PC4 -> MOT_R_IN2 -> AIN2                       */
/*                       PB5 -> MOT_R_IN3 -> BIN1                       */
/*                       PB4 -> MOT_R_IN4 -> BIN2                       */
/************************************************************************/

/************************************************************************************/
/*             Input               |               OUTPUT                |   Mode   */
/* ---------------------------------------------------------------------------------*/
/* nSLEEP| AIN1| AIN2| BIN1| BIN2  |   AOUT1 | AOUT2 | BOUT1 | BOUT2     |          */
/*   0   |  X  |  X  |  X  |   X   |    Hiz  |  Hiz  |  Hiz  |  Hiz      |   Sleep  */
/*   1   |  0  |  0  |             |    Hiz  |  Hiz  |                   |   Slide  */
/*   1   |  0  |  1  |             |     0   |   1   |                   |  Forward */
/*   1   |  1  |  0  |             |     1   |   0   |                   |  Reverse */
/*   1   |  1  |  1  |             |     0   |   0   |                   |   Brake  */
/*   1   |           |  0  |   0   |                 |  Hiz  |  Hiz      |   Slide  */
/*   1   |           |  0  |   1   |                 |   0   |   1       |  Forward */
/*   1   |           |  1  |   0   |                 |   1   |   0       |  Reverse */
/*   1   |           |  1  |   1   |                 |   0   |   0       |   Brake  */
/************************************************************************************/

#ifdef _USE_HW_STEP

	DEFINE_STEP_MOTOR(step_motor_left,
	PORTC, DDRC, PC1,   // AIN1: PC1
	PORTC, DDRC, PC0,   // AIN2: PC0
	PORTC, DDRC, PC3,   // BIN1: PC3
	PORTC, DDRC, PC2    // BIN2: PC2
	);

	DEFINE_STEP_MOTOR(step_motor_right,
	PORTC, DDRC, PC5,   // AIN1: PC5
	PORTC, DDRC, PC4,   // AIN2: PC4
	PORTB, DDRB, PB5,   // BIN1: PB5
	PORTB, DDRB, PB4    // BIN2: PB4
	);

	#if (_USE_STEP_MODE == _STEP_MODE_HALF)
		#define STEP_MASK 0x07
		#define STEP_PER_REV 40
	#elif (_USE_STEP_MODE == _STEP_MODE_FULL)
		#define STEP_MASK 0x03
		#define STEP_PER_REV 20
	#elif (_USE_STEP_MODE == _STEP_MODE_MICRO)
		#define STEP_MASK 0x0F
	#endif
	
	//max slew speed : 1500Hz mininmum -> 1/1500 ~= 666us
	#define SAFE_TORQUE_FREQ	800		//unit : Hz
	#define MAX_PERIOD_US		1200	//unit : us -> 1.2ms
	#define MIN_PERIOD_US		100000	//unit : us -> 100ms
	
	#define MAX_RPM				2250
	#define SAFE_MAX_RPM		1200
	#define MIN_RPM				0
	#define MIN_SPEED			0
	#define MAX_SPEED			100
	
#endif


//내뷰 유틸: 현재 step_idx에 맞춰 코일 출력 갱신
static void apply_coils(StepMotor *m)
{
	// AIN1
	if (STEP_TABLE[m->step_idx][0]) *(m->ain1_port) |=  (1 << m->ain1_pin);
	else                            *(m->ain1_port) &= ~(1 << m->ain1_pin);
	// AIN2
	if (STEP_TABLE[m->step_idx][1]) *(m->ain2_port) |=  (1 << m->ain2_pin);
	else                            *(m->ain2_port) &= ~(1 << m->ain2_pin);
	// BIN1
	if (STEP_TABLE[m->step_idx][2]) *(m->bin1_port) |=  (1 << m->bin1_pin);
	else                            *(m->bin1_port) &= ~(1 << m->bin1_pin);
	// BIN2
	if (STEP_TABLE[m->step_idx][3]) *(m->bin2_port) |=  (1 << m->bin2_pin);
	else                            *(m->bin2_port) &= ~(1 << m->bin2_pin);
}

// 초기화: 모든 입력 핀 출력으로 설정
void sm_init(StepMotor *m) 
{
	// DDR 설정: OUTPUT 모드 
	*(m->ain1_ddr) |=  (1 << m->ain1_pin);
	*(m->ain2_ddr) |=  (1 << m->ain2_pin);
	*(m->bin1_ddr) |=  (1 << m->bin1_pin);
	*(m->bin2_ddr) |=  (1 << m->bin2_pin);
	
	// 출력 초기화 : 모든 코일 off
	*(m->ain1_port) &= ~(1 << m->ain1_pin);
	*(m->ain2_port) &= ~(1 << m->ain2_pin);
	*(m->bin1_port) &= ~(1 << m->bin1_pin);
	*(m->bin2_port) &= ~(1 << m->bin2_pin);
	
	m->step_idx = 0;
	
	m->forward = (m == &step_motor_right) ? sm_reverse : sm_forward;
	m->reverse = (m == &step_motor_right) ? sm_forward : sm_reverse;
}

// 슬라이드(Coast)
void sm_slide(StepMotor *m) 
{
	*(m->ain1_port) &= ~(1 << m->ain1_pin);
	*(m->ain2_port) &= ~(1 << m->ain2_pin);
	*(m->bin1_port) &= ~(1 << m->bin1_pin);
	*(m->bin2_port) &= ~(1 << m->bin2_pin);
}

// 전진(Forward)
void sm_forward(StepMotor *m) 
{
	apply_coils(m);
	m->step_idx = (m->step_idx + 1) & STEP_MASK;  // 0~7 순환(half step) | 0~3 순환(full step)
}

// 후진(Reverse)
void sm_reverse(StepMotor *m) 
{
	 apply_coils(m);
	 m->step_idx = (m->step_idx + STEP_MASK) & STEP_MASK;  // 7 ~ 0 || 3 ~ 0
}

// 브레이크(Brake)
void sm_brake(StepMotor *m) 
{
	*(m->ain1_port) |=  (1 << m->ain1_pin);
	*(m->ain2_port) |=  (1 << m->ain2_pin);
	*(m->bin1_port) |=  (1 << m->bin1_pin);
	*(m->bin2_port) |=  (1 << m->bin2_pin);
}

/////////////////////////////////////////////////////////

void roe_sm_init(void)
{
	step_motor_left.init(&step_motor_left);
	step_motor_right.init(&step_motor_right);
}


void roe_sm_forward(void)
{
	step_motor_left.forward(&step_motor_left);
	step_motor_right.forward(&step_motor_right);	
}

void roe_sm_reverse(void)
{
	step_motor_left.reverse(&step_motor_left);
	step_motor_right.reverse(&step_motor_right);
}

void roe_sm_brake(void)
{
	step_motor_left.brake(&step_motor_left);
	step_motor_right.brake(&step_motor_right);
}

int constrain(int target_value, int min_value, int max_value)
{
	if(target_value > max_value)
	{
		return max_value;
	}
	else if(target_value < min_value)
	{
		return min_value;
	}
	else
	{
		return target_value;
	}
}

int32_t mapping(int target_value, int from_low, int from_high, int to_low, int to_high)
{
	target_value = constrain(target_value, from_low, from_high);
	return (target_value - from_low) * (to_high - to_low) / (from_high - from_low) + to_low;
}

uint32_t pwm_to_rpm(uint8_t speed_pwm)
{
	return mapping(speed_pwm, MIN_SPEED, MAX_SPEED, MIN_RPM, SAFE_MAX_RPM);
}

uint32_t rpm_to_period(uint16_t rpm)
{
	/*max_rpm(2250) = 1500(max_PPS) * 60(1min) / half-step(40°)*/
	/*But we have to secure safe freq*/
	/*safe_rpm(1200) = 800(safe_pps) * 60(1min) / half-step(40°)*/
	rpm = (rpm > SAFE_MAX_RPM) ? SAFE_MAX_RPM : rpm;
	rpm = (rpm == 0) ? 1 : rpm;
	
	/* period_us = min_to_us / (rpm * steps_per_revolution) */
	/* 40(steps_per_rev) = 360° / 9°(half-step angle) */
	/* 60000000(min_to_us) = 60(1minute to sec)* 1000(sec_to_ms) * 1000(ms_to_us) */
	return 60000000UL / ( (uint32_t)rpm * STEP_PER_REV);
}

void roe_sm_operate(void)
{
	uint16_t rpm = pwm_to_rpm(1);
	if (rpm == 0)
	{
		roe_sm_brake();
		return;
	}
	
	uint32_t period = rpm_to_period(rpm);
	uint32_t current_time = micros();
	static uint32_t prev_time = 0;
	
	if(current_time - prev_time >= period)
	{
		roe_sm_reverse();
		prev_time = current_time;
	}
}

void roe_operate_rogic(uint8_t m_pin, uint8_t speed, unsigned char m_dir)
{
	StepMotor *m;
	void (*dir_op)(StepMotor *);
	
	m = (m_pin == LEFT) ? &step_motor_left : &step_motor_right;
	
	if(speed == 0)
	{
		m->brake(m);
		return;
	}
	m->period_us = rpm_to_period(pwm_to_rpm(speed));
	
	dir_op = (m_dir == FORWARD) ? m->forward : m->reverse;
	
	uint32_t current_time = micros();
	if(current_time - m->last_period_us >= m->period_us)
	{
		dir_op(m);
		m->last_period_us = current_time;
	}
}

