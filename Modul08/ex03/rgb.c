/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:20:48 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/16 00:22:30 by cmetee-b         ###   ########.fr       */
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

void apa102_set_led(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
	spi_transmit(0xE0 | (brightness & 0x1F));
	spi_transmit(b);
	spi_transmit(g);
	spi_transmit(r);
}

void spi_init(void)
{
	DDRB |= (1 << PB5);  // SCK
	DDRB |= (1 << PB3);  // MOSI
	DDRB |= (1 << PB2);  // SS
	
	/* SPCR - SPI Control Register */
	SPCR = (1 << SPE) | (1 << MSTR);
}

void rgb_set_jauge(uint8_t level)
{
	apa102_start_frame();
	
	// D6 - Allumée si level >= 1
	if (level >= 1)
		apa102_set_led(5, 0x00, 0xFF, 0x00);
	else
		apa102_set_led(0, 0x00, 0x00, 0x00);
	
	// D7 - Allumée si level >= 2
	if (level >= 2)
		apa102_set_led(15, 0xFF, 0x00, 0x00);
	else
		apa102_set_led(0, 0x00, 0x00, 0x00);
	
	// D8 - Allumée si level >= 3
	if (level >= 3)
		apa102_set_led(5, 0xFF, 0xFF, 0x00);
	else
		apa102_set_led(0, 0x00, 0x00, 0x00);
	
	apa102_end_frame();
}

