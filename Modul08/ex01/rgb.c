/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:20:48 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 23:29:28 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>
#include <util/delay.h>

/*
** Tableau des couleurs
** Format: {r, g, b}
*/
typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color_t;

static const color_t colors[] = {
	{0xFF, 0x00, 0x00},  // Rouge
	{0x00, 0xFF, 0x00},  // Vert
	{0x00, 0x00, 0xFF},  // Bleu
	{0xFF, 0xFF, 0x00},  // Jaune
	{0x00, 0xFF, 0xFF},  // Cyan
	{0xFF, 0x00, 0xFF},  // Magenta
	{0xFF, 0xFF, 0xFF}   // Blanc
};

#define NB_COLORS (sizeof(colors) / sizeof(colors[0]))

/*
** Fonction statique pour transmettre un octet via SPI
** Datasheet ATmega328P Section 18.5.2 - SPDR: SPI Data Register
*/
static void spi_transmit(uint8_t data)
{
	SPDR = data;
	
	/*
	** Attente de fin de transmission
	** Section 18.5.1 - Bit 7 SPIF: SPI Interrupt Flag
	*/
	while (!(SPSR & (1 << SPIF)))
		;
}

/*
** Envoie la trame de début du protocole APA102
** Start Frame = 32 bits à 0
*/
static void apa102_start_frame(void)
{
	for (uint8_t i = 0; i < 4; i++)
		spi_transmit(0x00);
}

/*
** Envoie la trame de fin du protocole APA102
** End Frame = 32 bits à 1
*/
static void apa102_end_frame(void)
{
	for (uint8_t i = 0; i < 4; i++)
		spi_transmit(0xFF);
}

/*
** Configure une LED APA102
** Trame: [111BBBBB][Blue][Green][Red]
** brightness: 0-31 (luminosité)
*/
static void apa102_set_led(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
	spi_transmit(0xE0 | (brightness & 0x1F));
	spi_transmit(b);
	spi_transmit(g);
	spi_transmit(r);
}

void rgb_init(void)
{
	/*
	** Configuration des pins SPI en sortie
	** Datasheet Section 18.3 - SS, MOSI et SCK doivent être en OUTPUT
	*/
	DDRB |= (1 << PB5);  // SCK (pin 17)
	DDRB |= (1 << PB3);  // MOSI (pin 15)
	DDRB |= (1 << PB2);  // SS (requis pour mode maître)
	
	/*
	** SPCR - SPI Control Register
	** Section 18.5.1
	** SPE: SPI Enable
	** MSTR: Master mode
	** SPR1=0, SPR0=0: Clock = fosc/4 = 4MHz
	*/
	SPCR = (1 << SPE) | (1 << MSTR);
}

void rgb_set_color(uint8_t r, uint8_t g, uint8_t b)
{
	apa102_start_frame();
	
	/*
	** D6 avec la couleur demandée
	** Brightness fixé à 5 pour une luminosité modérée
	*/
	apa102_set_led(5, r, g, b);
	
	/*
	** D7 et D8 éteintes (LEDs en série)
	*/
	apa102_set_led(0, 0, 0, 0);
	apa102_set_led(0, 0, 0, 0);
	
	apa102_end_frame();
}

int main(void)
{
	uint8_t index = 0;

	rgb_init();
	
	while (1)
	{
		// Affiche la couleur courante
		rgb_set_color(colors[index].r, 
		              colors[index].g, 
		              colors[index].b);
		
		// Attend 1 seconde
		_delay_ms(1000);
		
		// Passe à la couleur suivante (boucle)
		index = (index + 1) % NB_COLORS;
	}
	return 0;
}
