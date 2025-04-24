/*
 * stepper.c
 *
 * Created: 2025-04-22 오후 6:33:18
 *  Author: RCY
 */ 

#include "stepper.h"

/*                              Motor (15BY25)                          */
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

#if _USE_STEP_HALF
	#define STEP_MASK 0x07
#else
	#define STEP_MASK 0x03
#endif

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
	
	m->forward = (m == &step_motor_left) ? sm_reverse : sm_forward;
	m->reverse = (m == &step_motor_left) ? sm_forward : sm_reverse;
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
	m->step_idx = (m->step_idx + 1) & STEP_MASK;  // 0~7 순환 | 0!3 순환
	apply_coils(m);
	_delay_us(500); // 속도 조절
}

// 후진(Reverse)
void sm_reverse(StepMotor *m) 
{
	 m->step_idx = (m->step_idx + 7) & STEP_MASK;  // -1 mod 8
	 apply_coils(m);
	 _delay_us(500);
}

// 브레이크(Brake)
void sm_brake(StepMotor *m) 
{
	*(m->ain1_port) |=  (1 << m->ain1_pin);
	*(m->ain2_port) |=  (1 << m->ain2_pin);
	*(m->bin1_port) |=  (1 << m->bin1_pin);
	*(m->bin2_port) |=  (1 << m->bin2_pin);
}

void sm_init_all(void)
{
	step_motor_left.init(&step_motor_left);
	step_motor_right.init(&step_motor_right);
}

void sm_operate(void)
{
	//for(uint8_t i = 0; i < 20; i++)
	//{
		//step_motor_left.forward(&step_motor_left);//이게 정회전임
		//step_motor_right.forward(&step_motor_right);	
	//}
	//_delay_ms(100);
	
	step_motor_left.forward(&step_motor_left);
	step_motor_right.forward(&step_motor_right);
	_delay_ms(10);
	
	//for(uint8_t i = 0; i < 20; i++)
	//{
		//step_motor_left.reverse(&step_motor_left);
		//step_motor_right.reverse(&step_motor_right);
	//}
	//_delay_ms(100);
}