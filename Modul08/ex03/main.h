#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
** Canal ADC pour le potentiomètre RV1
** D'après le schéma, RV1/ADC_POT est sur PC0 = ADC0
*/
#define POT_CHANNEL 0

/*
** Seuils pour la jauge (valeurs ADC 10 bits: 0-1023)
** 33% de 1023 ≈ 341
** 66% de 1023 ≈ 682
*/
#define THRESHOLD_33  341
#define THRESHOLD_66  682


/* SPI & APA102 & RGB */
void spi_init(void);
void spi_transmit(uint8_t data);
void apa102_start_frame(void);
void apa102_end_frame(void);
void apa102_set_led(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

/* ADC */
void adc_init(void);
uint16_t adc_read(uint8_t channel);

/*
** Allume progressivement Les LEDs
	** Jauge progressive:
	** level 0: ⚫⚫⚫ (toutes éteintes)
	** level 1: 🟢⚫⚫ (D6 seule)
	** level 2: 🟢🟢⚫ (D6 + D7)
	** level 3: 🟢🟢🟢 (toutes allumées)
*/
void rgb_set_jauge(uint8_t level);

#endif