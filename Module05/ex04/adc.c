/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 20:40:58 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 20:44:18 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// Datasheet ATmega328P : Section 23/24 - ADC

void adc_init(void)
{
    /* Configuration de la référence AVCC (page 257)
     * REFS0 = 1 : AVCC avec condensateur externe sur AREF
     */
    ADMUX = (1 << REFS0);
    
    /* Activation de l'ADC (page 258)
     */
    ADCSRA = (1 << ADEN);
    
    /* Configuration du prescaler 128 : 16MHz/128 = 125kHz
     * ADPS[2:0] = 111
     */
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
}

uint16_t adc_read(uint8_t channel)
{
    /* Sélection du canal ADC (0-7)
     * ADC0 (PC0) = RV1 potentiomètre
     */
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    /* Démarrer la conversion */
    ADCSRA |= (1 << ADSC);
    
    /* Attendre la fin de conversion */
    while (ADCSRA & (1 << ADSC));
    
    /* Lire le résultat 10 bits */
    return ADC;
}
