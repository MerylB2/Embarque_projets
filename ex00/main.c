/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 11:14:20 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/10/28 16:59:59 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>


void ft_delay_ms(void)
{
	// Boucle externe: 20 itérations
	for (uint8_t i = 0; i < 20; i++)
	{
		// Boucle interne: ~50000 itérations
		// Calcul approximatif: 20 * 50000 * 4 cycles ≈ 4M cycles
		// À 16MHz: 4M cycles / 16M Hz = 0.25s
		// On fait 2 fois cette boucle pour ~500ms
		for (uint16_t j = 0; j < 50000; j++)
		{
			// Boucle vide - le compilateur ne devrait pas l'optimiser
			// car nous n'utilisons pas -O3
			asm volatile("nop");
		}
	}
}

int main(void)
{
	// Configuration de PB1 (broche 9, LED D2) en sortie
	// DDRB = Data Direction Register pour le port B
	// Mettre le bit 1 à 1 pour configurer PB1 en OUTPUT
	// 0b00000010 = 0x02 = (1 << PB1)
	DDRB |= (1 << PB1);

	// Initialisation: LED éteinte au démarrage
	// PORTB = Port B Data Register
	// Mettre le bit 1 à 0 pour éteindre la LED
	PORTB &= ~(1 << PB1);

	// Boucle infinie - le programme ne doit jamais se terminer
	while (1)
	{
		// Toggle (inversion) de l'état de PB1 avec XOR
		// PINB = Port B Input Pins Register
		// Écrire 1 sur PINB toggle le bit correspondant dans PORTB
		// C'est une fonctionnalité hardware de l'ATmega328p
		// Pas de condition if/else nécessaire
		PINB = (1 << PB1);

		// Attente de 500ms
		// Avec toggle toutes les 500ms, on obtient:
		// - LED ON pendant 500ms
		// - LED OFF pendant 500ms
		// = Période de 1 seconde = Fréquence de 1Hz
		ft_delay_ms();
	}

	// Cette ligne ne sera jamais atteinte
	// Le main ne doit pas return (requis par l'exercice)
	return 0;
}


// // Fonction de délai
// void wait_centaines_ms(int centaines_ms)
// {
// 	for (int i = 0; i < centaines_ms; i++)
// 	{
// 		for (int j = 0; j < 2000; j++)
// 		{
// 			for (int k = 0; k < 200; k++)
// 			{
// 			   asm volatile("nop");
// 			}
// 		}
// 	}
// }

// int main(void)
// {
// 	DDRB |= (1 << PB1);
// 	PORTB &= ~(1 << PB1);
	
// 	while (1)
// 	{
// 		PORTB ^= (1 << PB1);
// 		wait_centaines_ms(5);  // 500ms
// 	}
	
// 	return 0;
// }
