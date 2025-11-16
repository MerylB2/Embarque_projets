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

#define UART_BAUDRATE 115200

/* UART */
void uart_init(void);
void uart_tx(char c);
void uart_printstr(const char *str);
char uart_rx(void);
// Vérifie si des données sont disponibles sur l'UART 
uint8_t uart_available(void);


/* SPI & APA102 & RGB */
void spi_init(void);
void spi_transmit(uint8_t data);
void apa102_start_frame(void);
void apa102_end_frame(void);
void apa102_set_led(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);


/* Définit la couleur d'une LED spécifique (D6, D7 ou D8) */
void rgb_set_led(uint8_t led_index, uint8_t r, uint8_t g, uint8_t b);

/*
** Fonction wheel() pour générer un arc-en-ciel
** 	 	pos: Position dans le cycle (0-255)
** 		led_index: Quelle LED contrôler (0-2)
*/
void wheel(uint8_t pos, uint8_t led_index);

/* Parsing */
void process_command(char *cmd);
void fullrainbow_mode(void);

/* Utiles */
void read_line(char *buffer, uint8_t max_len);
uint8_t hex_to_num(char c);
char to_upper(char c);
uint8_t parse_hex_byte(char h, char l);
uint8_t str_len(const char *str);
uint8_t str_cmp(const char *s1, const char *s2);
char num_to_hex(uint8_t num);
void print_hex_byte(uint8_t value);

#endif