/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:49:11 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/07 17:30:24 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/* 
 * Configuration matérielle (voir schéma ) :
 * - SW1 -> PD2 - pin 32 (INTO)
 * - LED_D1 -> connectée à PB0
 */

void init_led(void)
{
	DDRB |= (1 << PB0);
	PORTB &= ~(1 << PB0);
}

void setup_sw_interrupt(void)
{
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2); //pull_up interne
	
	// Configuration du type d'interruption INT0
	// Datasheet p.80: EICRA (External Interrupt Control Register A)
	// ISC01 ISC00 | Description
	//   0     0   | Low level génère une interruption
	//   0     1   | Tout changement génère une interruption
	//   1     0   | Falling edge (front descendant)
	//   1     1   | Rising edge (front montant)
	
	// On utilise falling edge (appui sur le bouton - front descendant)
	EICRA |= (1 << ISC01);
	EICRA &= ~(1 << ISC00);

	// Datasheet p.81 : EIMSK (External Interrupt Mask Register)
	// INT0 bit active l'interruption externe 0
	EIMSK |= (1 << INT0);

	sei();  // Set Enable Interrupts
}

// Datasheet p.74 : INT0_vect est le vecteur pour l'interruption externe 0
void __vector_1(void) __attribute__((signal, used));
void __vector_1(void)
{
	_delay_ms(50); 
	
	// Toggle de la LED D1
	// On inverse l'état de la LED à chaque interruption
	PORTB ^= (1 << PB0);
	
	// Note : Le flag d'interruption INTF0 est automatiquement effacé
	// par le hardware lors de l'entrée dans la routine d'interruption
	// Datasheet p.81 : "The flag is cleared when the interrupt routine is executed"
}

int main(void)
{
	init_led();
	setup_sw_interrupt();

	while (1)
	{}
	return 0;
}
