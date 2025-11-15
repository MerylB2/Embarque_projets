#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* SPI */
void spi_init(void);
void spi_transmit(uint8_t data);
void apa102_start_frame(void);
void apa102_end_frame(void);
void apa102_set_led(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

#endif