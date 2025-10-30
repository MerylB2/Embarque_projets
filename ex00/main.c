/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 11:14:20 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/10/29 19:15:33 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

/* 
*Boucles imbriquées (uint8_t + uint16_t) pour économiser la RAM
* 32 itérations externes × 50000 itérations internes × 5 cycles
* ≈ 8M cycles = 500ms à 16MHz
* Économe en RAM (3 bytes), code compact,
* rapide (arithmétique 8/16-bit native)
*/

/**
 * Délai basé sur comptage de cycles CPU
 * 
 * p.625_626 - Chapitre 37: Instruction Set Summary
 * - NOP: 1 cycle
 * - SBIW: 2 cycles  
 * - BRNE: 1-2 cycles
 */
void ft_delay_ms(void)
{
	// Section 8.1: SRAM Data Memory (2KB seulement)
	// Boucle externe: 32 itérations pour atteindre 500ms
	for (uint8_t i = 0; i < 32; i++)
	{
		// Boucle interne: 50000 itérations
		// 32 × 50000 × 5 cycles ≈ 8M cycles = 500ms
		for (uint16_t j = 0; j < 50000; j++)
		{
			// Chapitre 37: NOP = 1 cycle CPU
			__asm__ volatile("nop");
		}
	}
}

int main(void)
{
	//p.100 - Section 14.4.3 : "The DDxn bit in the DDRx Register 
	// selects the direction of this pin"
	// DDxn = 1 → Pin configured as output
	DDRB |= (1 << DDB1);
 
	// Section 14.4.2 : "If PORTxn is written logic one when 
	// the pin is configured as an output pin, the port pin 
	// is driven high"
	// Init LED éteinte
	PORTB &= ~(1 << PORTB1);
	
	while (1)
	{
		// Section 14.2.2: Toggling the Pin _ p.85
        // "Writing a logic one to PINxn toggles the value of 
        // PORTxn, independent on the value of DDRxn"
		// unique à l'ATmega328P !
		// Un XOR hardware automatique sans lecture préalable
		PINB = (1 << PINB1);
		
		// Délai 500ms
		ft_delay_ms();
	}
}
