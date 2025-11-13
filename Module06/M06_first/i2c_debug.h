#ifndef I2C_DEBUG_H
#define I2C_DEBUG_H

/*
 * Fonctions de debug optionnelles pour I2C
 * 
 * Ces fonctions permettent d'afficher des informations détaillées
 * sur l'état du bus I2C après chaque opération.
 * 
 * Inspiré des bonnes pratiques de debug I2C.
 */

/* Debug après i2c_start() */
void i2c_start_debug(void);

/* Debug après i2c_write() d'une donnée */
void i2c_write_debug(void);

/* Debug après i2c_write() d'une adresse Write */
void i2c_write_address_debug(void);

/* Debug après i2c_write() d'une adresse Read */
void i2c_read_address_debug(void);

/* Debug après i2c_read() */
void i2c_read_debug(uint8_t data);

#endif