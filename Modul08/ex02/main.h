#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_ON_TIME 250    // Durée d'allumage de chaque LED (ms)


/* SPI & APA102 */
void spi_init(void);
void spi_transmit(uint8_t data);
void apa102_start_frame(void);
void apa102_end_frame(void);
void apa102_set_led(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

/*
** Allume uniquement une LED parmi D6, D7, D8
**   led_index: 0 pour D6, 1 pour D7, 2 pour D8
**   red: Intensité du rouge (0-255)
**   green: Intensité du vert (0-255)
**   blue: Intensité du bleu (0-255)
** Les autres LEDs sont éteintes
*/
void rgb_set_one_led(uint8_t led_index, uint8_t r, uint8_t g, uint8_t b);


#endif