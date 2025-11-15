/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:34:13 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/16 00:18:58 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


int main(void)
{
	uint16_t adc_value;
	uint8_t gauge_level;
	
	adc_init();
	spi_init();
	
	while (1)
	{
		// Lecture de la valeur du potentiomètre
		adc_value = adc_read(POT_CHANNEL);
		
		/*
		** Conversion de la valeur ADC en niveau de jauge (0-3)
		** 
		** ADC 0-340:     niveau 0 (toutes éteintes) - position basse
		** ADC 341-681:   niveau 1 (D6 seule) - 33%
		** ADC 682-1023:  niveau 2 (D6+D7) - 66%
		** ADC 1023:      niveau 3 (toutes) - 100%
		*/
		if (adc_value < THRESHOLD_33)
			gauge_level = 0;
		else if (adc_value < THRESHOLD_66)
			gauge_level = 1;
		else if (adc_value < 1023)
			gauge_level = 2;
		else
			gauge_level = 3;
		
		// Affichage de la jauge
		rgb_set_jauge(gauge_level);
		
		// Petit délai pour éviter le scintillement
		_delay_ms(50);
	}
	
	return 0;
}
