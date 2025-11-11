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


uint8_t read_ADC_value(void);
void ADC_init_potentiometre(void);

void spi_master_init();
void set_leds(uint32_t d6, uint32_t d7, uint32_t d8);

#endif
