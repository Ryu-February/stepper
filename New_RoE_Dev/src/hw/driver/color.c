/*
 * color.c
 *
 * Created: 2025-05-08 오전 11:13:44
 *  Author: RCY
 */ 

#include "color.h"
#include "i2c.h"

//static void (*color_id_cb)(uint8_t) = 0;
//
//void color_read_id_async(void (*cb)(uint8_t)) {
	//color_id_cb = cb;
	//i2c_read_reg_async_cb(TCS34725_ADDR, TCS34725_COMMAND_BIT | 0x12, color_id_cb);
//}


bool color_init(void)
{
	//soft_i2c_bus1_init();
	
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
	
	if (!soft_i2c1_write8(TCS34725_ADDR, TCS34725_COMMAND_BIT | TCS34725_CONTROL,
							TCS34725_CTRL_REG__4X_GAIN))
	{
		return false;						
	}

	// Set integration time
	return soft_i2c1_write8(TCS34725_ADDR, 
							TCS34725_COMMAND_BIT | TCS34725_ATIME,
							TCS34725_INTEGRATIONTIME);
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
	uint8_t status = color_read8(TCS34725_STATUS);
	if (status & TCS34725_STATUS_AVALID) // AVALID가 1이면 데이터 준비 완료
	{
		uint8_t low  = color_read8(reg);
		uint8_t high = color_read8(reg + 1);
		return ((uint16_t)high << 8) | low;
	}
    return 0;
}

/**
 * Read clear, red, green, blue channels
 */
bool color_read_rgbc(uint16_t *c, uint16_t *r, uint16_t *g, uint16_t *b)
{
    *c = color_read16(TCS34725_CDATAL);
    *r = color_read16(TCS34725_RDATAL);
    *g = color_read16(TCS34725_GDATAL);
    *b = color_read16(TCS34725_BDATAL);
    return true;
}
