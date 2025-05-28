/*
 * color.c
 *
 * Created: 2025-05-08 오전 11:13:44
 *  Author: RCY
 */ 

#include "uart.h"
#include "color.h"
#include "i2c.h"

//static void (*color_id_cb)(uint8_t) = 0;
//
//void color_read_id_async(void (*cb)(uint8_t)) {
	//color_id_cb = cb;
	//i2c_read_reg_async_cb(TCS34725_ADDR, TCS34725_COMMAND_BIT | 0x12, color_id_cb);
//}

unsigned char color_read = false;

bool color_init(void)
{
	if (!soft_i2c1_write8(TCS34725_ADDR, TCS34725_COMMAND_BIT | TCS34725_ENABLE, TCS34725_ENABLE_PON))
	{
		return false;
	}
	delay_ms(3);

	// Enable RGBC
	if (!soft_i2c1_write8(TCS34725_ADDR, TCS34725_COMMAND_BIT | TCS34725_ENABLE,
							TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN))
	{
		return false;							
	}
	
	delay_ms(3);
	
	// Set integration time
	if (!soft_i2c1_write8(TCS34725_ADDR, TCS34725_COMMAND_BIT | TCS34725_ATIME,
							TCS34725_INTEGRATIONTIME__700MS))
	{
		return false;
	}
	
	// Set RGBC gain
	//uint16_t c = color_read16(TCS34725_CDATAL);
	//uint8_t gain = 0;
	
	//if(c > 52000)	   gain = TCS34725_CTRL_REG__1X_GAIN;
	//else if(c > 36000) gain = TCS34725_CTRL_REG__4X_GAIN;
	//else               gain = TCS34725_CTRL_REG__16X_GAIN;
	
	if (!soft_i2c1_write8(TCS34725_ADDR, TCS34725_COMMAND_BIT | TCS34725_CONTROL, 
							TCS34725_CTRL_REG__4X_GAIN))
	{
		return false;
	}
	
	return true;
}

/**
 * Write a single byte to a sensor register
 */
bool color_write8(uint8_t reg, uint8_t value)
{
    return soft_i2c1_write8(TCS34725_ADDR, TCS34725_COMMAND_BIT | reg, value);
}

/**
 * Read a single byte from a sensor register
 */
uint8_t color_read8(uint8_t reg)
{
    return soft_i2c1_read8(TCS34725_ADDR, TCS34725_COMMAND_BIT | reg);
}

/**
 * Read a 16-bit word (low byte then high byte)
 */
uint16_t color_read16(uint8_t reg)
{
	uint8_t low  = color_read8(reg);
	uint8_t high = color_read8(reg + 1);
	
	return ((uint16_t)high << 8) | low;
}

/**
 * Read clear, red, green, blue channels
 */
bool color_read_rgbc(uint16_t *c, uint16_t *r, uint16_t *g, uint16_t *b)
{
	uint8_t status = color_read8(TCS34725_STATUS);
	if(!(status & TCS34725_STATUS_AVALID))// AVALID가 1이면 데이터 준비 완료(integration time)
	{
		return false;
	}

    *c = color_read16(TCS34725_CDATAL);
    *r = color_read16(TCS34725_RDATAL);
    *g = color_read16(TCS34725_GDATAL);
    *b = color_read16(TCS34725_BDATAL);
	
    return true;
}

uint16_t color_calc_hue(uint16_t *c, uint16_t *r, uint16_t *g, uint16_t *b)
{
	uint32_t sum = *r + *g + *b;
	
	float rn = (float)*r / sum;
	float gn = (float)*g / sum;
	float bn = (float)*b / sum;
	
	float mx = fmaxf(rn, gn);
	float maxc = fmaxf(mx, bn);
	float minc = fminf(rn, gn);
		  minc = fminf(minc, bn);
		  
	float delta = maxc - minc;
	
	if(delta == 0)
	{
		return 0;
	}
	
	float hue;
	
	if(maxc == rn)
	{
		hue = 60 * fmodf((gn - bn) / delta, 6);
	}
	else if(maxc == gn)
	{
		hue = 60 * (((bn - rn) / delta) + 2);
	}
	else
	{
		hue = 60 * (((rn - gn) / delta) + 4);
	}
	
	return hue < 0 ? hue + 360 : hue;
}

color_t color_from_hue(uint16_t hue)
{
	if(hue < 30 || hue > 330)
	{
		return COLOR_RED;
	}
	else if(hue >= 60 && hue < 180)
	{
		return COLOR_GREEN;
	}
	else if(hue >= 180 && hue <= 300)
	{
		return COLOR_BLUE;
	}
	
	return COLOR_UNKNOWN;
}