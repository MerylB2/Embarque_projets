#ifndef I2C_DEBUG_H
# define I2C_DEBUG_H

# include <avr/io.h>
# include <util/twi.h>

void	i2c_start_debug(void);
void	i2c_write_debug(void);
void	i2c_read_debug(uint8_t transmit);
void	i2c_write_address_debug(void);
void	i2c_read_address_debug(void);

#endif
