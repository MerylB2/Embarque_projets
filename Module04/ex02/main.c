/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 15:58:25 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/07 16:08:47 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


// Variable globale pour le compteur (0-15 pour 4 bits)
volatile uint8_t counter = 0;


void config_leds(void)
{
	DDRB |= (1 << LED_D1);
	DDRB |= (1 << LED_D2);
	DDRB |= (1 << LED_D3);
	DDRB |= (1 << LED_D4);
	
	PORTB &= ~((1 << LED_D1) | (1 << LED_D2) | (1 << LED_D3) | (1 << LED_D4));
}

void update_leds(uint8_t value)
{	
	value &= 0x0F;       			// Limite la valeur à 4 bits (0-15)
	
	PORTB &= ~LED_MASK; 			// Éteint d'abord toutes les LEDs
	
	PORTB |= (value & 0b00000111);  // les bits 0, 1, 2  -> D1, D2, D3
        
	if (value & (1 << 3))
		PORTB |= (1 << LED_D4);
}

void setup_sw_interrupt(void)
{
	// Configure SW1 sur INT0 (PD2) en entrée avec pull-up
	SW1_DDR &= ~(1 << SW1_PIN);
	SW1_PORT |= (1 << SW1_PIN);
	
	EICRA |= (1 << ISC01);   // Falling edge
	EICRA &= ~(1 << ISC00);
	
	// Active l'interruption INT0 -> EIMSK
	EIMSK |= (1 << INT0);
	
	/*
	 * Configuration SW2 sur PD4 avec Pin Change Interrupt
	 * PD4 fait partie du groupe PCINT2 (PCINT20)
	 */
	
	// Configure SW2 en entrée avec pull-up
	SW2_DDR &= ~(1 << SW2_PIN);
	SW2_PORT |= (1 << SW2_PIN);
	
	// Active Pin Change Interrupt pour le groupe PCINT2 (PORTD)
	// PCICR (Pin Change Interrupt Control Register)
	PCICR |= (1 << PCIE2);
	
	// Active PCINT20 (PD4) dans le masque
	// PCMSK2 (Pin Change Mask Register 2)
	PCMSK2 |= (1 << PCINT20);
	
	sei();
}

/*
 * Interruption INT0 - Bouton SW1 (Incrément)
 * isr.h : ISR_EXTERNAL_0 correspond à __vector_1
 */
ISR_EXTERNAL_0
{

// Vérifie que le bouton est pressé (pin = 0)
	if (!(PIND & (1 << SW1_PIN)))
	{
		_delay_ms(10);

		if (!(PIND & (1 << SW1_PIN)))
		{
			counter++;
			if (counter > 15)
				counter = 0;
				
			update_leds(counter);
    
			// Attendre que le bouton soit relâché
			while (!(PIND & (1 << SW1_PIN)))
				;
			// Attendre encore un peu après le relâchement
			_delay_ms(10);
		}
	}
}

/*
 * Interruption Pin Change INT2 - Bouton SW2 (Décrément)
 * isr.h : ISR_PIN_CHANGE_2 correspond à __vector_5
 */
ISR_PIN_CHANGE_2
{
	if (!(PIND & (1 << SW2_PIN)))
	{
		_delay_ms(10);
		
		if (!(PIND & (1 << SW2_PIN)))
		{
			if (counter == 0)
				counter = 15;
			else
				counter--;
			
			update_leds(counter);
			
			while (!(PIND & (1 << SW2_PIN)))
				;
			_delay_ms(10);
		}
	}
}

int main(void)
{
	config_leds();
	setup_sw_interrupt();
	
	update_leds(counter);

	while (1)
	{}
}
