/*
 * led.c
 *
 * Created: 2025-04-24 오후 4:42:49
 *  Author: RCY
 */ 

#include "uart.h"
#include "timer.h"
#include "led.h"

static const rgb_led_t led = 
{
	&PORTB,
	&DDRB,
	LED_RED_MASK,
	LED_GREEN_MASK,
	LED_BLUE_MASK,
	LED_COLOR_MASK
};

//RGB_LED				|    COLOR_LED     |
//R LED -> PB1(OC1A)	| WHITE LED -> PB0 |
//G LED -> PB2(OC1B)	|				   |
//B LED -> PB3(OC2A)	|				   |


static const struct
{
	rgb_color_t c;
	uint8_t r, g, b;
}
led_map[] =
{
	{ CLR_BLACK,        0,   0,   0 },
	{ CLR_RED,        255,   0,   0 },
	{ CLR_ORANGE,     255, 165,   0 },
	{ CLR_YELLOW,     255, 255,   0 },
	{ CLR_GREEN,        0, 255,   0 },
	{ CLR_LIGHT_GREEN,144, 238, 144 },
	{ CLR_CYAN,         0, 255, 255 },
	{ CLR_LIGHT_BLUE, 173, 216, 230 },
	{ CLR_BLUE,         0,   0, 255 },
	{ CLR_INDIGO,      75,   0, 130 },
	{ CLR_VIOLET,     238, 130, 238 },
	{ CLR_PINK,       255, 105, 180 },
	{ CLR_WHITE,      255, 255, 255 }
};

#define LED_MAP_LEN		(sizeof(led_map) / sizeof(led_map[0]))

void led_init(void)
{
#if (_USE_LED_POL && !_USE_LED_IRQ)
	*(led.ddr)  |= (led.red_mask) | (led.green_mask) | (led.blue_mask);
	*(led.port) |= (led.red_mask) | (led.green_mask) | (led.blue_mask);//led->pullup resistor
#elif (!_USE_LED_POL && _USE_LED_IRQ)
	timer_pwm_init_channel(TIMER1_ID, PWM_CH_A);
	timer_pwm_init_channel(TIMER1_ID, PWM_CH_B);
	timer_pwm_init_channel(TIMER2_ID, PWM_CH_A);
	*(led.port) |= (led.red_mask) | (led.green_mask) | (led.blue_mask);//led->pullup resistor
#endif
	/*MOSFET N-channel gate is connected to this pin(PORTB0)*/
	/*This LED is mounted next to the color sensor*/
	*(led.ddr)	|= (led.color_mask);
	*(led.port) |= (led.color_mask);
}

void led_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	/*OCR value must be inverted due to pull-up resistor*/
	timer_pwm_set_duty(TIMER1_ID, PWM_CH_A, 255 - r);
	timer_pwm_set_duty(TIMER1_ID, PWM_CH_B, 255 - g);
	timer_pwm_set_duty(TIMER2_ID, PWM_CH_A, 255 - b);
}

void led_set_color(uint8_t rgb_color)
{
	
#if (_USE_LED_POL && !_USE_LED_IRQ)
	
	if(rgb_color & RGB_COLOR_RED)
	{
		*(led.port) &= ~led.red_mask;
	}
	else
	{
		*(led.port) |= led.red_mask;
	}
	
	if(rgb_color & RGB_COLOR_GREEN)
	{
		*(led.port) &= ~led.green_mask;
	}
	else
	{
		*(led.port) |= led.green_mask;
	}
	
	if(rgb_color & RGB_COLOR_BLUE)
	{
		*(led.port) &= ~led.blue_mask;
	}
	else
	{
		*(led.port) |= led.blue_mask;
	}
	
#elif (!_USE_LED_POL && _USE_LED_IRQ)

	//for(uint8_t i = 0; i < LED_MAP_LEN; i++)
	//{
		//if(led_map[i].c == rgb_color)
		//{
			//led_set_rgb(led_map[i].r, led_map[i].g, led_map[i].b);
		//}
	//}
	led_set_rgb(led_map[rgb_color].r, led_map[rgb_color].g, led_map[rgb_color].b);
	
#endif
}