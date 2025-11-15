/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spi.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:01:48 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 23:16:44 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
** Configuration SPI (ATmega328P Datasheet Section 18)
** - MOSI (PB3/pin 15) → Data Out vers APA102
** - SCK (PB5/pin 17) → Clock vers APA102
** - Mode Maître requis pour contrôler les LEDs
*/

void spi_init(void)
{
	/*
	** Configuration des pins SPI en sortie
	** Datasheet Section 18.3 - SS, MOSI et SCK doivent être en OUTPUT en mode maître
	*/
	DDRB |= (1 << PB5);  // SCK en sortie (pin 17)
	DDRB |= (1 << PB3);  // MOSI en sortie (pin 15)
	DDRB |= (1 << PB2);  // SS en sortie (requis pour mode maître stable)
	
	/*
	** SPCR - SPI Control Register (Datasheet Section 18.5.1)
	** Bit 6 – SPE: SPI Enable
	** Bit 4 – MSTR: Master/Slave Select (1 = Master)
	** Bit 1,0 – SPR1,SPR0: SPI Clock Rate Select
	**   SPR1=0, SPR0=0 → fosc/4 = 4MHz (adapté pour APA102, max 20MHz)
	*/
	SPCR = (1 << SPE) | (1 << MSTR);
}

void spi_transmit(uint8_t data)
{
	/*
	** Transmission d'un octet via SPI
	** Datasheet Section 18.5.2 - SPDR: SPI Data Register
	*/
	SPDR = data;
	
	/*
	** Attente de fin de transmission
	** Datasheet Section 18.5.1 - Bit 7 SPIF: SPI Interrupt Flag
	** SPIF est mis à 1 quand la transmission est complète
	*/
	while (!(SPSR & (1 << SPIF)))
		;
}

void apa102_start_frame(void)
{
	/*
	** APA102 Protocol: Start Frame = 32 bits à 0
	** Voir datasheet APA102 pour le protocole
	*/
	for (uint8_t i = 0; i < 4; i++)
		spi_transmit(0x00);
}

void apa102_end_frame(void)
{
	/*
	** APA102 Protocol: End Frame = 32 bits à 1
	** Nécessaire pour valider l'affichage
	*/
	for (uint8_t i = 0; i < 4; i++)
		spi_transmit(0xFF);
}

void apa102_set_led(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b)
{
	/*
	** Trame LED APA102 (32 bits):
	** - Byte 1: 111BBBBB (3 bits à 1 + 5 bits de luminosité 0-31)
	** - Byte 2: Blue
	** - Byte 3: Green
	** - Byte 4: Red
	*/
	spi_transmit(0xE0 | (brightness & 0x1F));  // 0xE0 = 0b11100000
	spi_transmit(b);
	spi_transmit(g);
	spi_transmit(r);
}
