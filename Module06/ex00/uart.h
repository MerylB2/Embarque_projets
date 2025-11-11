#ifndef UART_H
# define UART_H

# include <avr/io.h>

void	init_uart(uint16_t ubbr);
void	uart_printstr(const char * str);
void	uart_tx(const char c);
	
#endif
