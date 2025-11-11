#include "uart.h"
#include "main.h"

void	print_hex_value(unsigned char c)
{
	if (c >= 16)
	{
		print_hex_value(c / 16);
		print_hex_value(c % 16);
	}
	else
	{
		if (c >= 10)
			uart_tx(c - 10 + 'A');
		else
			uart_tx(c + '0');
	}
	return ;
}

void	uart_printdeca(uint32_t n)
{
	if (n >= 10)
	{
		uart_printdeca(n / 10);
		uart_printdeca(n % 10);
	}
	else
		uart_tx(n + '0');
}

/* void	print_value(uint32_t temp[3], uint32_t rh[3]) */
/* { */
/* 	float	medium_temp = (temp[0] + temp[1] + temp[2]) / (float)(3); */
/* 	float	medium_rh = (rh[0] + rh[1] + rh[2]) / (float)(3); */

/* 	/1* char	res[8]; *1/ */
/* 	uart_printstr("Temperature: "); */
/* 	/1* dtostrf(medium_temp, 8, 2, res); *1/ */
/* 	/1* uart_printstr(res); *1/ */
/* 	print_deca(medium_temp); */
/* 	uart_printstr(".C, "); */
/* 	uart_printstr("Humidity: "); */
/* 	/1* dtostrf(medium_rh, 8, 2, res); *1/ */
/* 	/1* uart_printstr(res); *1/ */
/* 	print_deca(medium_rh); */
/* 	uart_printstr("%\r\n"); */
/* } */

void	uart_tx(const char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void	uart_printstr(const char * str)
{
	while (*str)
	{
		uart_tx(*str);
		str++;
	}
}

void	init_uart(uint16_t ubbr)
/* init of usart0 with double speed mode in transmit mode only. baud rate is 115 200 */
{
	SET(UCSR0A, U2X0);
	SET(UCSR0B, TXEN0);

	SET(UCSR0C, UCSZ00);
	SET(UCSR0C, UCSZ01);
	/* UART will use 8 bit transmission */

	UBRR0H = (uint8_t)(ubbr >> 8);
	UBRR0L = (uint8_t)ubbr;
}
