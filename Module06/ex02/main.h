#ifndef MAIN_H
# define MAIN_H

# ifndef F_CPU 
#  define F_CPU 16000000UL
# endif

# define BOUNCE_DURATION 20

# define SET_OUTPUT(x, a) DDR##x |= (1 << P##x##a)
# define SET_INPUT(x, a) DDR##x &= ~(1 << P##x##a)

# define PORT_TOGGLE(x, a) PIN##x ^= (1 << P##x##a)

# define SET(reg, bit) reg |= (1 << bit)
# define CLEAR(reg, bit) reg &= ~(1 << bit)

# define TURN_ON(x, a) PORT##x |= (1 << P##x##a)
# define TURN_OFF(x, a) PORT##x &= ~(1 << P##x##a)

# include <avr/io.h>
# include <util/twi.h>
# include <util/delay.h>

#endif
