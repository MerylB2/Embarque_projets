/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:20:48 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/16 02:59:48 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// Variables globales pour stocker les couleurs de chaque LED
static uint8_t led_colors[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

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

/*
** Met à jour l'affichage complet des 3 LEDs
*/
static void update_leds(void)
{
	apa102_start_frame();
	
	for (uint8_t i = 0; i < 3; i++)
	{
		uint8_t brightness;
		if (led_colors[i][0] == 0 && led_colors[i][1] == 0 && led_colors[i][2] == 0)
			brightness = 0;
		else
			brightness = 2;
		apa102_set_led(brightness, led_colors[i][0], led_colors[i][1], led_colors[i][2]);
	}
	apa102_end_frame();
}

void spi_init(void)
{
	DDRB |= (1 << PB5);  // SCK
	DDRB |= (1 << PB3);  // MOSI
	DDRB |= (1 << PB2);  // SS
	
	/* SPCR - SPI Control Register */
	SPCR = (1 << SPE) | (1 << MSTR);
	// Initialisation: toutes les LEDs éteintes
	for (uint8_t i = 0; i < 3; i++)
	{
		led_colors[i][0] = 0;
		led_colors[i][1] = 0;
		led_colors[i][2] = 0;
	}
	update_leds();
}


void rgb_set_led(uint8_t led_index, uint8_t r, uint8_t g, uint8_t b)
{
	if (led_index < 3)
	{
		led_colors[led_index][0] = r;
		led_colors[led_index][1] = g;
		led_colors[led_index][2] = b;
		update_leds();
	}
}

void wheel(uint8_t pos, uint8_t led_index)
{
	pos = 255 - pos;
	
	if (pos < 85)
	{
		rgb_set_led(led_index, 255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		rgb_set_led(led_index, 0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		rgb_set_led(led_index, pos * 3, 255 - pos * 3, 0);
	}
}
