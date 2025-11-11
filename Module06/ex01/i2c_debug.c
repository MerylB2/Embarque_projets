#include "i2c_debug.h"
#include "main.h"
#include "uart.h"

void	i2c_start_debug(void)
{
	if ((uint8_t)(TWSR) == 0x08)
		uart_printstr("START condition has been transmitted\n\r");
	else if ((uint8_t)(TWSR) == 0x10)
		uart_printstr("Repeated START condition has been transmitted\n\r");
	else
	{
		uart_printstr("Unexcepted Error Flag: TWSR = 0x");	
		print_hex_value((uint8_t)(TWSR));
		uart_printstr("\r\n");
	}
}

void	i2c_write_debug(void)
{
	uart_printstr("Data byte has been transmitted. ");
	if ((uint8_t)(TWSR) == 0x28)
		uart_printstr("ACK received\r\n");
	else if ((uint8_t)(TWSR) == 0x30)
		uart_printstr("NOT ACK received\r\n");
	else if ((uint8_t)(TWSR) == 0x38)
		uart_printstr("Arbitration lost in data byte.\r\n");
	else
	{
		uart_printstr("Unexcepted Error Flag: TWSR = 0x");
		print_hex_value((uint8_t)(TWSR));
		uart_printstr("\r\n");	
	}
}

void	i2c_read_debug(uint8_t transmit)
{	
	if (!temp_sensor_finished && (transmit & (1 << 7)))
		uart_printstr("Waiting for completing measurement\r\n");
	else if (!temp_sensor_finished)
		temp_sensor_finished = 1;
	else if ((uint8_t)(TWSR) == 0x50 || (uint8_t)(TWSR) == 0x58)
	{	
		uart_printstr("Value of read byte is: 0x");
		print_hex_value(transmit);
		if ((uint8_t)(TWSR) == 0x50)
			uart_printstr(" . ACK returned\r\n");
		else
			uart_printstr(" . NOT ACK returned\r\n");
	}
	else
	{
		uart_printstr("Unexpected Error Flag: TWSR = 0x");
		print_hex_value((uint8_t)(TWSR));
		uart_printstr("\r\n");
	}
}

void	i2c_write_address_debug(void)
{
	uart_printstr("SLA+W has been transmitted. ");
	if ((uint8_t)(TWSR) == 0x18)
		uart_printstr("ACK received\r\n");
	else if ((uint8_t)(TWSR) == 0x20)
		uart_printstr("NOT ACK received\r\n");
	else if ((uint8_t)(TWSR) == 0x38)
		uart_printstr("Arbitration lost in SLA+W\r\n");
	else
	{
		uart_printstr("Unexcepted Error Flag: TWSR = 0x");
		print_hex_value((uint8_t)(TWSR));
		uart_printstr("\r\n");
	}	
}

void	i2c_read_address_debug(void)
{
	uart_printstr("SLA+R has been transmitted. ");
	if ((uint8_t)(TWSR) == 0x40)
		uart_printstr("ACK received\r\n");
	else if ((uint8_t)(TWSR) == 0x48)
		uart_printstr("NOT ACK received\r\n");
	else if ((uint8_t)(TWSR) == 0x10)
		uart_printstr("Arbitration lost in SLA+R.\r\n");
	else
	{
		uart_printstr("Unexcepted Error Flag: TWSR = 0x");
		print_hex_value((uint8_t)(TWSR));
		uart_printstr("\r\n");
	}
}
