#ifndef MAIN_H
# define MAIN_H

# ifndef F_CPU
#  define F_CPU 16000000UL
# endif
# ifndef UART_BAUDERATE
#  define UART_BAUDERATE 115200
# endif

#include <stdint.h>

# define SET_BIT(REGISTER, BIT) REGISTER |= (1 << BIT)
# define CLEAR_BIT(REGISTER, BIT) REGISTER &= ~(1 << BIT)
# define REV_BIT(REGISTER, BIT) REGISTER ^= (1 << BIT)

# define MYUBRR F_CPU / 8 / UART_BAUDERATE - 1 // Formula for Asynchronous double speed mode (20.3.1)

void  uart_init(unsigned int ubrr);
void  uart_printstr(const char *str);
void  uart_tx(unsigned char c);
char  uart_rx(void);
void  uart_putnbr(uint16_t n);

void	putnbr_hexa(uint32_t nb);
char is_in(char c, const char *str);
uint32_t atoi_hexa(const char *str);
int ft_strcmp(const char *s1, const char *s2);
int	is_printable(unsigned char c);
int	ft_strncmp(const char *s1, const char *s2, unsigned int n);
unsigned int ft_strlen(const char *s);

void spi_master_init();
void set_leds(uint32_t d6, uint32_t d7, uint32_t d8);

#endif
