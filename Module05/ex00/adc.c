/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 18:19:27 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/08 18:13:49 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

// https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
// Configuration de l'ADC en 8 bits avec AVCC comme référence

void adc_init(void)
{
    /* Section 24.9 Register Description p.257
     * Configuration de la référence AVCC (registre ADMUX)
     * Bit 7:6 – REFS[1:0]: Reference Selection Bits :
     *   " These bits select the voltage reference for the ADC, 
     *    as shown in Table 24-3."
     * REFS0 = 1, REFS1 = 0 : AVCC avec condensateur externe sur AREF
     */
    ADMUX = (1 << REFS0);
    
    
    /* Sélection du canal ADC0 (PC0 où est connecté RV1)
     * Bits 3:0 – MUX[3:0]: Analog Channel Selection Bits :
     *    " The value of these bits selects which analog inputs are connected to the ADC.
     *    See Table 24-4 for details "
     * MUX[3:0] = 0000 pour ADC0 (page 258)
     * Pas besoin de configurer MUX car par défaut à 0
     */
    
    /* Ajustement à gauche pour lecture 8 bits
     * Bit 5 – ADLAR: ADC Left Adjust Result
     * Section 24.9.3.2 (p.259) ADLAR = 1: les 8 bits MSB sont dans ADCH
     */
    ADMUX |= (1 << ADLAR);
    
    /* Activation de l'ADC (page 258, registre ADCSRA)
     * Bit 7 – ADEN: ADC Enable
     * ADEN = 1 : Active l'ADC
     */
    ADCSRA = (1 << ADEN);
    
    /* Configuration du prescaler (page 259)
     * Bits 2:0 – ADPS[2:0]: ADC Prescaler Select Bits
     * Pour F_CPU = 16MHz, on veut fADC entre 50kHz et 200kHz
     * Prescaler 128 : 16MHz/128 = 125kHz (optimal)
     * ADPS[2:0] = 111 pour prescaler 128
     */
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t adc_read(void)
{
    /* Bit 6 – ADSC: ADC Start Conversion
     * ADSC = 1 : Démarre une conversion
     */
    ADCSRA |= (1 << ADSC);
    
    /* Attendre la fin de conversion
     * ADSC repasse à 0 quand la conversion est terminée
     */
    while (ADCSRA & (1 << ADSC))
        ;
    
    /* Lire le résultat 8 bits (En mode ADLAR=1 -> ADCH */
    return ADCH;
}
