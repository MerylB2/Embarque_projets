#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <util/delay.h>

void led_init(void);
void led_set_var(uint16_t adc_value);
void wheel(uint8_t pos);
void set_rgb(uint8_t r, uint8_t g, uint8_t b);
void adc_init(void);
uint16_t adc_read(uint8_t channel);

#endif