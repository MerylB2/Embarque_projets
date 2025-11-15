/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:20:48 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 23:47:14 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void spi_transmit(uint8_t data)
{
	SPDR = data;
	
	while (!(SPSR & (1 << SPIF)))
		;
}

void apa102_start_frame(void)
{
	for (uint8_t i = 0; i < 4; i++)
		spi_transmit(0x00);
}

void apa102_end_frame(void)
{
	for (uint8_t i = 0; i < 4; i++)
		spi_transmit(0xFF);
}

/*
** Configure une LED APA102
** Trame: [111BBBBB][Blue][Green][Red]
** brightness: 0-31 (luminosité)
*/

void apa102_set_led(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
	spi_transmit(0xE0 | (brightness & 0x1F));
	spi_transmit(b);
	spi_transmit(g);
	spi_transmit(r);
}

void spi_init(void)
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

void rgb_set_one_led(uint8_t led_index, uint8_t r, uint8_t g, uint8_t b)
{
	apa102_start_frame();
	
	/*
	** Les LEDs APA102 sont en série: D6 -> D7 -> D8
	** Il faut envoyer une trame pour chaque LED
	*/
	for (uint8_t i = 0; i < 3; i++)
	{
		if (i == led_index)
		{
			// LED allumée avec brightness 10
			apa102_set_led(10, r, g, b);
		}
		else
		{
			// LED éteinte
			apa102_set_led(0, 0, 0, 0);
		}
	}
	
	apa102_end_frame();
}

