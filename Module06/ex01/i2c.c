#include "main.h"
#include "i2c.h"
#include "uart.h"
#include "i2c_debug.h"

void	i2c_init(void)
{
	/* so we wan 100khz on i2c bus and we have this equation: SCLfrq = F_CPU / (16 + 2 * TWBR * prescaler) */
	/* Hence, I set up TWBR at 72 and an prescaler of 1. which doesn't need set up because it's 2 bits cleared*/
	TWBR = 72;
	/* SDA is on port PC4 and SCL is ont port PC5 */
	SET_INPUT(C, 4);
	SET_INPUT(C, 5);

	SET(TWCR, TWEN);
	/* Bit to Enable TWI operation. I am activating TWI interface */
	_delay_ms(100);
	/* Wait need in man to power up the sensor */
	SET(TWCR, TWEA);
	/* ACK signal will be sent : I can confirm if my data is correctly transmitted on the TWI bus*/
	if (DEBUG)
		uart_printstr("I2C has been initialized\n\r");
}

void	i2c_start(uint8_t address, uint8_t mode)
{
	/* MCU is the master, T Sensor is the slave. I need to send message asking for a start on the bus and with address of T Sensor */
	/* Step one: write what I want to do */
	SET(TWCR, TWSTA);
	SET(TWCR, TWEA);

	/* Step two: write the TWINT bit to start communication */
	SET(TWCR, TWINT);

	/* Step three: wait for the start condition to be correctly transmit */
	/* When the TWINT flag is set (so becomes = 1), it means the start condition has been transmitted and we can read change in status */
	while (!(TWCR & (1 << TWINT))) ;
	if (DEBUG || ((uint8_t)(TWSR) != 0x08 && (uint8_t)(TWSR) != 0x10))
		i2c_start_debug();

	i2c_address(address, mode);
}

void	i2c_stop(void)
{
	/* Step one: write that I will send a stop condition: */
	SET(TWCR, TWSTO);

	/* Step two: clear the TWINT to send the stop condition */
	SET(TWCR, TWINT);
	if (DEBUG)
		uart_printstr("Stop condition has been transmitted\n\r");
}


void	i2c_write(unsigned char data)
{
	TWDR = data;
	SET(TWCR, TWEA);
	/* uart_printstr("doing...\n\r"); */
	SET(TWCR, TWINT);

	while (!(TWCR & (1 << TWINT))) ;
	if (DEBUG)
		i2c_write_debug();
}

void	i2c_read(void)
{
	SET(TWCR, TWINT);
	while (!(TWCR & (1 << TWINT))) ;
	char transmit = TWDR;
	if (DEBUG || ((uint8_t)(TWSR) != 0x50 && (uint8_t)(TWSR) != 0x58))
		i2c_read_debug(transmit);
	else 
	{	
		if (!temp_sensor_finished && !(transmit & (1 << 7)))
			temp_sensor_finished = 1;
		else
		{
			print_hex_value(transmit);
			uart_tx(' ');
		}
		
	}
}

void	i2c_address(uint8_t address, uint8_t mode)
/* If mode = 0 -> write mode, if mode = 1 -> read mode */
{	
	TWDR = address + mode;
	CLEAR(TWCR, TWSTA);
	SET(TWCR, TWINT);
	/* Write the address plus the byte for read or write mode. Than send it on the bus */
	while (!(TWCR & (1 << TWINT))) ;
	if ((DEBUG || (uint8_t)(TWSR) != 0x18) && !mode)
		i2c_write_address_debug();
	if ((DEBUG || (uint8_t)(TWSR) != 0x40) && mode)
		i2c_read_address_debug();
}

