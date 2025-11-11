#ifndef UART_H
# define UART_H

# include <avr/io.h>
# include <stdlib.h>

void	init_uart(uint16_t ubbr);
void	uart_printstr(const char * str);
void	uart_tx(const char c);
void	print_hex_value(unsigned char c);
void	uart_printdeca(uint32_t	n);

void	print_value(uint32_t temp[3], uint32_t rh[3]);

#endif
