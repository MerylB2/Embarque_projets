/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:52:10 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 23:18:17 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int main(void)
{
	spi_init();
	
	while (1)
	{
		// Start Frame
		apa102_start_frame();
		
		/*
		** Allumer D6 en ROUGE
		** Brightness: 1-2 (faible pour ne pas éblouir)
		** Rouge: 255, Vert: 0, Bleu: 0
		*/
		apa102_set_led(2, 255, 0, 0);
		
		/*
		** D7 et D8 éteintes (car LEDs en série)
		** Il faut envoyer des trames pour toutes les LEDs
		*/
		apa102_set_led(0, 0, 0, 0);  // D7 éteinte
		apa102_set_led(0, 0, 0, 0);  // D8 éteinte
		
		// End Frame
		apa102_end_frame();
		
		_delay_ms(100);
	}
	
	return 0;
}
