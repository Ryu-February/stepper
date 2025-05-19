/*
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

// Default integration time
#define TCS34725_INTEGRATIONTIME	0xC0  // 154 ms

bool color_init(void);
bool color_write8(uint8_t reg, uint8_t value);
uint8_t color_read8(uint8_t reg);
uint16_t color_read16(uint8_t reg);
bool color_read_rgbc(uint16_t *c, uint16_t *r, uint16_t *g, uint16_t *b);

#endif

#endif /* COLOR_H_ */