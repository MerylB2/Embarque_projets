#include "main.h"
#include "i2c.h"
#include "uart.h"


void	print_data(uint32_t rh[3], uint32_t temp[3], uint8_t rotation, uint8_t begin)
{
	uint16_t	tmp;

	tmp = 0;
	uart_printstr("temperature: ");
	temp[rotation] = (temp[rotation] / 5242.88f) - 50;
	for (int i = 0; i < begin; i++)
		tmp += (temp[i]);
	tmp /= begin;
	uart_printdeca(tmp);
	uart_printstr(" .C\r\n");

	tmp = 0;
	uart_printstr("humidity: ");
	rh[rotation] = (rh[rotation] / 10485.76f);
	for (int i = 0; i < begin; i++)
		tmp += (rh[i]);
	tmp /= begin;
	uart_printdeca(tmp);
	uart_printstr(" %\n\r");
}

void	read_value(uint32_t *rh, uint32_t *temp)
{
	uint16_t 	buff[3];

	buff[0] = i2c_read();
	buff[1] = i2c_read();
	buff[2] = i2c_read();
	*rh = ((uint32_t)buff[0] << 12) | ((uint32_t)buff[1] << 4) | (buff[2] >> 4);

	buff[0] = buff[2];
	buff[1] = i2c_read();
	CLEAR(TWCR, TWEA);
	buff[2] = i2c_read();
	*temp = ((uint32_t)(buff[0] & 0x0F) << 16) | ((uint32_t)buff[1] << 8) | buff[2];
}

uint8_t	check_init(void)
{
	uint8_t	buff;

	i2c_start(TEMP_SENSOR_ADDRESS << 1, 1);
	CLEAR(TWCR, TWEA);
	buff = i2c_read();
	i2c_stop();
	if (!(buff >> 3) & 0x01)
		return (1);
	return (0);
}

void	init_sensor(void)
{
	i2c_start(TEMP_SENSOR_ADDRESS << 1, 0);
	i2c_write(0xBE);
	i2c_write(0x08);
	i2c_write(0x00);
	i2c_stop();
	_delay_ms(10);
}

int	main(void)
{
	uint32_t	temp[3] = {0, 0, 0};
	uint32_t	rh[3] = {0, 0, 0};
	uint8_t		rotation = 0;
	uint8_t		begin = 1;

	init_uart(F_CPU / (8 * 115200) - 1);
	i2c_init();
	_delay_ms(40);
	if (check_init())
		init_sensor();
	while (1) 
	{
		uart_printstr("\r\n");
		i2c_start(TEMP_SENSOR_ADDRESS << 1, 0);
		i2c_write(0xAC);
		i2c_write(0x33);
		i2c_write(0x00);
		i2c_stop();
		_delay_ms(80);
		while (1)
		{
			i2c_start(TEMP_SENSOR_ADDRESS << 1, 1);
			if (!((i2c_read() >> 7) & 0x01))
				break ;
			_delay_ms(80);
		}
		read_value(&rh[rotation], &temp[rotation]);
		print_data(rh, temp, rotation, begin);
		rotation = (rotation + 1) % 3;
		_delay_ms(2000);
		if (begin < 3)
			begin++;
	}
}
