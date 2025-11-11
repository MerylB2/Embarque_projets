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

#endif
