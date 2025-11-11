#include "i2c.h"
#include "main.h"
#include "uart.h"

void	i2c_init(void)
{
	/* so we wan 100khz on i2c bus and we have this equation: SCLfrq = F_CPU / (16 + 2 * TWBR * prescaler) */
	/* Hence, I set up TWBR at 72 and an prescaler of 1. which doesn't need set up because it's 2 bits cleared*/
	TWBR = 72;
	/* SDA is on port PC4 and SCL is ont port PC5 */
	SET_INPUT(C, 4);
	SET_INPUT(C, 5);
	
	SET(TWCR, TWEN);
	SET(TWCR, TWEA);
	uart_printstr("I2C has been initialized\n\r");
}

void	i2c_start(void)
{
	/* MCU is the master, T Sensor is the slave. I need to send message asking for a start on the bus and with address of T Sensor */
	/* Step one: write what I want to do */
	SET(TWCR, TWSTA);

	/* Step two: write the TWINT bit to start communication */
	SET(TWCR, TWINT);

	/* Step three: wait for the start condition to be correctly transmit */
	while ((uint8_t)(TWSR) != 0x08);
	uart_printstr("Start condition has been transmitted\n\r");
}

void	i2c_stop(void)
{
	/* Step one: write that I will send a stop condition: */
	SET(TWCR, TWSTO);

	/* Step two: clear the TWINT to send the stop condition */
	SET(TWCR, TWINT);
	uart_printstr("Stop condition has been transmitted\n\r");
}
