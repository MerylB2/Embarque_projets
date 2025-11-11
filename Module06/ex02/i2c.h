#ifndef I2C_H
# define I2C_H

# include <avr/io.h>
# include <util/twi.h>

# define TEMP_SENSOR_ADDRESS 0x38
# ifndef DEBUG
#  define DEBUG 0
# endif
# define ACK 1
# define NACK 0

void	i2c_init(void);
void	i2c_start(uint8_t address, uint8_t mode);
void	i2c_stop(void);
void	i2c_write(unsigned char data);
uint8_t	i2c_read(void);

void	i2c_address(uint8_t address, uint8_t mode);


#endif
