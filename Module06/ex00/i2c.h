#ifndef I2C_H
# define I2C_H

# include <avr/io.h>
# include <util/twi.h>

void	i2c_init(void);
void	i2c_start(void);
void	i2c_stop(void);

#endif
