#include "main.h"
#include "i2c.h"
#include "uart.h"

volatile uint8_t	temp_sensor_finished;

int	main(void)
{
	init_uart(F_CPU / (8 * 115200) - 1);
	i2c_init();
	_delay_ms(40);
	i2c_start(TEMP_SENSOR_ADDRESS << 1, 1);
	i2c_read();
	while (1) 
	{
		
		if (!temp_sensor_finished)
		{
			uart_printstr("Asking sensor init\r\n");
			i2c_start(TEMP_SENSOR_ADDRESS << 1, 0);
			i2c_write(0xBE);
			i2c_write(0x08);
			i2c_write(0x00);
			_delay_ms(10);
		}
		uart_printstr("\r\n");
		i2c_start(TEMP_SENSOR_ADDRESS << 1, 0);
		i2c_write(0xAC);
		i2c_write(0x33);
		i2c_write(0x00);
		i2c_stop();
		_delay_ms(80);
		i2c_start(TEMP_SENSOR_ADDRESS << 1, 1);
		while (!temp_sensor_finished)
		{
			i2c_read();
			if (temp_sensor_finished)
				break ;
			_delay_ms(80);
		}
		for (int i = 0; i < 6; i++)
			i2c_read();
		CLEAR(TWCR, TWEA);
		i2c_read();
		i2c_stop();
		_delay_ms(2000);
	}
}
