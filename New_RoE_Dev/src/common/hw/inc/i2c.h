/*
 * i2c.h
 *
 * Created: 2025-05-08 오전 11:56:01
 *  Author: RCY
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "hw_def.h"

#ifdef _USE_HW_I2C

	#define I2C_SPEED_100KHZ			72
	#define I2C_SPEED_200KHZ			32
	#define I2C_SPEED_400KHZ			12
	
	enum
	{
		WRITE	= 1,
		READ	= 2	
	};
	//#define WRITE						1
	//#define READ						2
	
	void i2c_init(void);
	
	#if (_USE_I2C_MODE == _I2C_MODE_IRQ)
	
	void i2c_write_reg_async(uint8_t slave_addr, uint8_t reg, uint8_t data);
	void i2c_read_reg_async(uint8_t slave_addr, uint8_t reg);
	
	extern volatile uint8_t i2c_flag;  // 0: 진행 중, 1: 완료
	extern volatile uint8_t i2c_data;  // 읽은 데이터
	
	#elif (_USE_I2C_MODE == _I2C_MODE_SOFT)
	
	#define BUS1_SCL_PORT	PORTD
	#define BUS1_SCL_DDR	DDRD
	#define BUS1_SCL_PIN	PIND
	#define BUS1_SCL_BIT	PORTD7
	
	#define BUS1_SDA_PORT	PORTD
	#define BUS1_SDA_DDR	DDRD
	#define BUS1_SDA_PIN	PIND
	#define BUS1_SDA_BIT	PORTD6
	
	#define BUS2_SCL_PORT	PORTD
	#define BUS2_SCL_DDR	DDRD
	#define BUS2_SCL_PIN	PIND
	#define BUS2_SCL_BIT	PORTD5
	
	#define BUS2_SDA_PORT	PORTD
	#define BUS2_SDA_DDR	DDRD
	#define BUS2_SDA_PIN	PIND
	#define BUS2_SDA_BIT	PORTD4
	
	// Timing
	static inline void i2c_delay(void) { delay_us(5); }//i2c clock speed 200kHz

	// Bus-specific start/stop
	void soft_i2c_bus1_start(void);
	void soft_i2c_bus1_stop(void);
	void soft_i2c_bus2_start(void);
	void soft_i2c_bus2_stop(void);

	// Bus-specific byte transfer
	void soft_i2c_bus1_write_byte(uint8_t data);
	void soft_i2c_bus2_write_byte(uint8_t data);
	uint8_t soft_i2c_bus1_read_byte(bool ack);
	uint8_t soft_i2c_bus2_read_byte(bool ack);

	// High-level register access
	bool soft_i2c1_write8(uint8_t addr, uint8_t reg, uint8_t data);
	uint8_t soft_i2c1_read8(uint8_t addr, uint8_t reg);

	bool soft_i2c2_write8(uint8_t addr, uint8_t reg, uint8_t data);
	uint8_t soft_i2c2_read8(uint8_t addr, uint8_t reg);
	
	#endif
	
#endif

#endif /* I2C_H_ */