/*
 * i2c.c
 *
 * Created: 2025-05-08 오전 11:55:48
 *  Author: RCY
 */ 

#include "i2c.h"

#if (_USE_I2C_MODE == _I2C_MODE_IRQ)

volatile uint8_t i2c_flag = 0;
volatile uint8_t i2c_data = 0;

static uint8_t slave_addr = 0;
static uint8_t reg_addr = 0;
static uint8_t write_data = 0;
static uint8_t mode = 0;  // 0: none, 1: write, 2: read
static uint8_t state = 0;

#endif


void i2c_init(void)
{
	#if (_USE_I2C_MODE == _I2C_MODE_POL)
	
	//DDRC = (1 << I2C_SDA) | (1 << I2C_SCL);		//이거 출력포트로 설정하는 게 맞음(풀업이면 할 필요 없는 듯)
	
	TWSR0 = (0 << TWPS0) | (0 << TWPS1);			//if(TWPS == 00) -> prescaler = 1
	TWBR0 = 32;//72(100kHz), 32(200kHz), 12(400kHz)	//SCL = 16,000,000 / (16 + (2 * TWBR0) * prescaler) -> ~ KHz
	TWCR0 = (1 << TWEN) | (1 << TWEA);				//TWEA - TWI Enable Acknowledge, TWEN - TWI Enable
	TWCR0 |= (1 << TWIE);
	
	#elif (_USE_I2C_MODE == _I2C_MODE_IRQ)
	
	TWSR0 = (0 << TWPS0) | (0 << TWPS1);			//if(TWPS == 00) -> prescaler = 1
	TWBR0 = 32;//72(100kHz), 32(200kHz), 12(400kHz)	//SCL = 16,000,000 / (16 + (2 * TWBR0) * prescaler) -> ~ KHz
	TWCR0 = (1 << TWEN) | (1 << TWIE);
	
	#elif (_USE_I2C_MODE == _I2C_MODE_SOFT)
	
	//I2C Bus is in a High status(IDLE)
	
	BUS1_SCL_DDR  |= (1 << BUS1_SCL_BIT);
	BUS1_SDA_DDR  |= (1 << BUS1_SDA_BIT);
	BUS1_SCL_PORT |= (1 << BUS1_SCL_BIT);
	BUS1_SDA_PORT |= (1 << BUS1_SDA_BIT);
	
	BUS2_SCL_DDR  |= (1 << BUS2_SCL_BIT);
	BUS2_SDA_DDR  |= (1 << BUS2_SDA_BIT);
	BUS2_SCL_PORT |= (1 << BUS2_SCL_BIT);
	BUS2_SDA_PORT |= (1 << BUS2_SDA_BIT);
	
	//Used i2c_delay(5us) to mimic the target I2C clock speed
	
	#endif
	sei();
}

#if (_USE_I2C_MODE == _I2C_MODE_SOFT)

void soft_i2c_start(volatile uint8_t *SDA_DDR, volatile uint8_t *SDA_PORT, volatile uint8_t *SCL_PORT, uint8_t SDA_BIT, uint8_t SCL_BIT)
{
	*SDA_PORT |= (1 << SDA_BIT);
	*SCL_PORT |= (1 << SCL_BIT);
	i2c_delay();
	*SDA_PORT &= ~(1 << SDA_BIT);
	i2c_delay();
	*SCL_PORT &= ~(1 << SCL_BIT);
	i2c_delay();
}
void soft_i2c_bus1_start(void)
{
	soft_i2c_start(&BUS1_SDA_DDR, &BUS1_SDA_PORT, &BUS1_SCL_PORT, BUS1_SDA_BIT, BUS1_SCL_BIT);

}
void soft_i2c_bus2_start(void)
{
	soft_i2c_start(&BUS2_SDA_DDR, &BUS2_SDA_PORT, &BUS2_SCL_PORT, BUS2_SDA_BIT, BUS2_SCL_BIT);
}

void soft_i2c_stop(volatile uint8_t *SDA_DDR, volatile uint8_t *SDA_PORT, volatile uint8_t *SCL_PORT, uint8_t SDA_BIT, uint8_t SCL_BIT)
{
	*SDA_PORT &= ~(1 << SDA_BIT);
	i2c_delay();
	*SCL_PORT |= (1 << SCL_BIT);
	i2c_delay();
	*SDA_PORT |= (1 << SDA_BIT);
	i2c_delay();
}
void soft_i2c_bus1_stop(void)
{
	soft_i2c_stop(&BUS1_SDA_DDR, &BUS1_SDA_PORT, &BUS1_SCL_PORT, BUS1_SDA_BIT, BUS1_SCL_BIT);
}
void soft_i2c_bus2_stop(void)
{
	soft_i2c_stop(&BUS2_SDA_DDR, &BUS2_SDA_PORT, &BUS2_SCL_PORT, BUS2_SDA_BIT, BUS2_SCL_BIT);
}

void soft_i2c_write_byte(volatile uint8_t *SDA_DDR, volatile uint8_t *SDA_PORT, volatile uint8_t *SCL_PORT, volatile uint8_t *SDA_PIN, uint8_t SDA_BIT, uint8_t SCL_BIT, uint8_t data) {
	for (uint8_t i = 0; i < 8; i++) 
	{
		if (data & 0x80) *SDA_PORT |= (1 << SDA_BIT);
		else             *SDA_PORT &= ~(1 << SDA_BIT);
		data <<= 1;
		i2c_delay();
		*SCL_PORT |= (1 << SCL_BIT);
		i2c_delay();
		*SCL_PORT &= ~(1 << SCL_BIT);
		i2c_delay();
	}
	// ACK
	*SDA_DDR &= ~(1 << SDA_BIT);
	i2c_delay();
	*SCL_PORT |= (1 << SCL_BIT);
	i2c_delay();
	*SCL_PORT &= ~(1 << SCL_BIT);
	i2c_delay();
	*SDA_DDR |= (1 << SDA_BIT);
}
void soft_i2c_bus1_write_byte(uint8_t data)
{
	 soft_i2c_write_byte(&BUS1_SDA_DDR, &BUS1_SDA_PORT, &BUS1_SCL_PORT, &BUS1_SDA_PIN,
	 BUS1_SDA_BIT, BUS1_SCL_BIT, data);
}
void soft_i2c_bus2_write_byte(uint8_t data)
{
	soft_i2c_write_byte(&BUS2_SDA_DDR, &BUS2_SDA_PORT, &BUS2_SCL_PORT, &BUS2_SDA_PIN,
	BUS2_SDA_BIT, BUS2_SCL_BIT, data);
}

uint8_t soft_i2c_read_byte(volatile uint8_t *SDA_DDR, volatile uint8_t *SDA_PORT, volatile uint8_t *SCL_PORT, volatile uint8_t *SDA_PIN, uint8_t SDA_BIT, uint8_t SCL_BIT, bool ack) {
	uint8_t data = 0;
	*SDA_DDR &= ~(1 << SDA_BIT);
	for (uint8_t i = 0; i < 8; i++) 
	{
		data <<= 1;
		*SCL_PORT |= (1 << SCL_BIT);
		i2c_delay();
		if (*SDA_PIN & (1 << SDA_BIT)) data |= 1;
		*SCL_PORT &= ~(1 << SCL_BIT);
		i2c_delay();
	}
	*SDA_DDR |= (1 << SDA_BIT);
	if (ack) *SDA_PORT &= ~(1 << SDA_BIT);
	else     *SDA_PORT |= (1 << SDA_BIT);
	i2c_delay();
	*SCL_PORT |= (1 << SCL_BIT);
	i2c_delay();
	*SCL_PORT &= ~(1 << SCL_BIT);
	i2c_delay();
	return data;
}
uint8_t soft_i2c_bus1_read_byte(bool ack)
{
	 return soft_i2c_read_byte(&BUS1_SDA_DDR, &BUS1_SDA_PORT, &BUS1_SCL_PORT, &BUS1_SDA_PIN,
	 BUS1_SDA_BIT, BUS1_SCL_BIT, ack);
}
uint8_t soft_i2c_bus2_read_byte(bool ack)
{
	return soft_i2c_read_byte(&BUS2_SDA_DDR, &BUS2_SDA_PORT, &BUS2_SCL_PORT, &BUS2_SDA_PIN,
	BUS2_SDA_BIT, BUS2_SCL_BIT, ack);
}

// High-level register access
bool soft_i2c1_write8(uint8_t addr, uint8_t reg, uint8_t data) 
{
	soft_i2c_bus1_start();
	soft_i2c_bus1_write_byte(addr << 1);
	soft_i2c_bus1_write_byte(reg);
	soft_i2c_bus1_write_byte(data);
	soft_i2c_bus1_stop();
	return true;
}

uint8_t soft_i2c1_read8(uint8_t addr, uint8_t reg) 
{
	uint8_t val;
	soft_i2c_bus1_start();
	soft_i2c_bus1_write_byte(addr << 1);
	soft_i2c_bus1_write_byte(reg);
	soft_i2c_bus1_start();
	soft_i2c_bus1_write_byte((addr << 1) | 1);
	val = soft_i2c_bus1_read_byte(false);
	soft_i2c_bus1_stop();
	return val;
}

bool soft_i2c2_write8(uint8_t addr, uint8_t reg, uint8_t data) 
{	
	soft_i2c_bus2_start();
	soft_i2c_bus2_write_byte(addr << 1);
	soft_i2c_bus2_write_byte(reg);
	soft_i2c_bus2_write_byte(data);
	soft_i2c_bus2_stop();
	return true;
}

uint8_t soft_i2c2_read8(uint8_t addr, uint8_t reg) 
{
	uint8_t val;
	soft_i2c_bus2_start();
	soft_i2c_bus2_write_byte(addr << 1);
	soft_i2c_bus2_write_byte(reg);
	soft_i2c_bus2_start();
	soft_i2c_bus2_write_byte((addr << 1) | 1);
	val = soft_i2c_bus2_read_byte(false);
	soft_i2c_bus2_stop();
	return val;
}


#elif (_USE_I2C_MODE == _I2C_MODE_IRQ)

void i2c_write_reg_async(uint8_t addr, uint8_t reg, uint8_t data)
{
	if(mode != 0)
	{
		return;
	}
	
	slave_addr = (addr << 1);
	reg_addr = reg;
	write_data = data;
	mode = WRITE;
	state = 0;
	i2c_flag = 0;
	
	TWCR0 = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
}

void i2c_read_reg_async(uint8_t addr, uint8_t reg)
{
	if(mode != 0)
	{
		return;
	}
	
	slave_addr = (addr << 1);
	reg_addr = reg;
	mode = READ;
	state = 0;
	i2c_flag = 0;
	
	TWCR0 = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
}

ISR(TWI0_vect)
{
	switch (TWSR0 & 0xF8)
	{
		case 0x08: // START
		case 0x10: // Repeated START
		{
			TWDR0 = (state == 2) && (mode == 2) ? (slave_addr | 0x01) : slave_addr;
			TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
			break;
		}
		case 0x18: // SLA+W ACK
		{
			TWDR0 = (state == 0) ? reg_addr : write_data;
			state++;
			TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
			break;
		}
		case 0x28: // Data written ACK
		{
			if (mode == 1 && state == 2) 
			{
				i2c_flag = 1;
				TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
				mode = 0;
			} 
			else if (mode == 2 && state == 1) 
			{
				state = 2;
				TWCR0 = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
			}
			break;
		}
		case 0x40: // SLA+R ACK
		{
			TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWIE); // NACK for single-byte
			break;
		}
		case 0x58: // Data read NACK
		{
			i2c_data = TWDR0;
			i2c_flag = 1;
			TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
			mode = 0;
			break;
		}
		default:
		{
			i2c_flag = 1;
			TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
			mode = 0;
			break;
		}
	}
}

#endif