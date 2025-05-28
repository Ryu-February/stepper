/*
 * led.h
 *
 * Created: 2025-04-24 오후 4:42:58
 *  Author: RCY
 */ 


#ifndef LED_H_
#define LED_H_


#include "hw_def.h"

#ifdef _USE_HW_LED

#define _USE_LED_POL	0
#define _USE_LED_IRQ	1

#define LED_COLOR_PIN	PB0//for color sensor -> mosfet n channel gate
#define LED_COLOR_MASK	(1 << LED_COLOR_PIN)

#define LED_RED_PIN		PB1
#define LED_GREEN_PIN	PB2
#define LED_BLUE_PIN	PB3

#define LED_RED_MASK	(1 << LED_RED_PIN)
#define LED_GREEN_MASK	(1 << LED_GREEN_PIN)
#define LED_BLUE_MASK	(1 << LED_BLUE_PIN)

#define RGB_COLOR_RED	(1 << 0)
#define RGB_COLOR_GREEN (1 << 1)
#define RGB_COLOR_BLUE	(1 << 2)

#if _USE_LED_POL
#define RGB_OFF			0
#define RGB_RED			(RGB_COLOR_RED)
#define RGB_GREEN		(RGB_COLOR_GREEN)
#define RGB_BLUE		(RGB_COLOR_BLUE)
#define RGB_YELLOW		(RGB_COLOR_RED | RGB_COLOR_GREEN)
#define RGB_MAGENTA		(RGB_COLOR_RED | RGB_COLOR_BLUE)
#define RGB_CYAN		(RGB_COLOR_GREEN | RGB_COLOR_BLUE)
#define RGB_WHITE		(RGB_COLOR_RED | RGB_COLOR_GREEN | RGB_COLOR_BLUE)
#endif



typedef struct 
{
	volatile uint8_t *port;	//IO 레지스터(PORTB) 주소값 할당해야 해서 포인터 변수로 선언
	volatile uint8_t *ddr;	//IO 레지스터(DDRB) 주소값 할당해야 해서 포인터 변수로 선언
	uint8_t red_mask;
	uint8_t green_mask;
	uint8_t blue_mask;
	uint8_t color_mask;
}rgb_led_t;

typedef enum
{
	CLR_BLACK = 0,
	CLR_RED,
	CLR_ORANGE,
	CLR_YELLOW,
	CLR_GREEN,
	CLR_LIGHT_GREEN,
	CLR_CYAN,
	CLR_LIGHT_BLUE,
	CLR_BLUE,
	CLR_INDIGO,
	CLR_VIOLET,
	CLR_PINK,
	CLR_WHITE,
	CLR_COUNT
}rgb_color_t;



//extern rgb_led_t led;

void led_init(void/*const rgb_led_t *led*/);
void led_set_color(uint8_t rgb_color);

#endif

#endif /* LED_H_ */