/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:34:13 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 23:47:43 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int main(void)
{
	spi_init();
	
	while (1)
	{
		/*
		** Allumage successif des 3 LEDs en rgb
		** D6 (index 0), D7 (index 1), D8 (index 2)
		*/
		
		// D6 allumée pendant 250ms
		rgb_set_one_led(0, 0xFF, 0x00, 0x00);
		_delay_ms(LED_ON_TIME);
		
		// D7 allumée pendant 250ms
		rgb_set_one_led(1, 0x00, 0xFF, 0x00);
		_delay_ms(LED_ON_TIME);
		
		// D8 allumée pendant 250ms
		rgb_set_one_led(2, 0x00, 0x00, 0xFF);
		_delay_ms(LED_ON_TIME);
		
		/*
		** Pause de 250ms pour compléter le cycle d'1 seconde
		** (3 × 250ms allumage + 1 × 250ms pause = 1000ms)
		*/
		rgb_set_one_led(3, 0x00, 0x00, 0x00);  // Toutes éteintes
		_delay_ms(LED_ON_TIME);
	}
	
	return 0;
}