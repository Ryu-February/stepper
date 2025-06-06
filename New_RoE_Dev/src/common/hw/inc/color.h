﻿/*
 * color.h
 *
 * Created: 2025-05-08 오전 11:13:53
 *  Author: RCY
 */ 


#ifndef COLOR_H_
#define COLOR_H_

#include "hw_def.h"

#ifdef _USE_HW_COLOR

#define TCS34725_ADDR				0x29
#define TCS34725_ID_REG				0x12
#define TCS34725_DEVICE_ID			0x44
#define TCS34725_COMMAND_BIT		0x80

#define TCS34725_STATUS				0x13
#define TCS34725_STATUS_AVALID		(1 << 0)

#define TCS34725_ENABLE				0x00
#define TCS34725_ENABLE_PON			0x01
#define TCS34725_ENABLE_AEN			0x02

#define TCS34725_ATIME				0x01
#define TCS34725_CONTROL			0x0F

#define TCS34725_CDATAL				0x14
#define TCS34725_RDATAL				0x16
#define TCS34725_GDATAL				0x18
#define TCS34725_BDATAL				0x1A

// Default integration time | equation(ms) : 256 - (Integration time / 2.4)
typedef enum
{
	TCS34725_INTEGRATIONTIME__2MS	= 0xFF, // 2.4 ms
	TCS34725_INTEGRATIONTIME__24MS	= 0xF6, // 24  ms
	TCS34725_INTEGRATIONTIME__101MS = 0xD5, // 101 ms
	TCS34725_INTEGRATIONTIME__154MS = 0xC0, // 154 ms
	TCS34725_INTEGRATIONTIME__700MS = 0x00	// 700 ms
}tcs34725_atime_integ_t;

typedef enum
{
	TCS34725_CTRL_REG__1X_GAIN	  =	0x00,
	TCS34725_CTRL_REG__4X_GAIN	  =	0x01,
	TCS34725_CTRL_REG__16X_GAIN	  =	0x02,
	TCS34725_CTRL_REG__64X_GAIN	  =	0x03
}tcs34725_ctrl_gain_t;


typedef enum
{
	COLOR_UNKNOWN = 0,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,	
}color_t;

bool color_init(void);
bool color_write8(uint8_t reg, uint8_t value);
uint8_t color_read8(uint8_t reg);
uint16_t color_read16(uint8_t reg);
bool color_read_rgbc(uint16_t *c, uint16_t *r, uint16_t *g, uint16_t *b);
uint16_t color_calc_hue(uint16_t *c, uint16_t *r, uint16_t *g, uint16_t *b);
color_t color_from_hue(uint16_t hue);

#endif

#endif /* COLOR_H_ */