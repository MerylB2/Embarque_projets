/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 18:19:27 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/08 18:06:42 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

// https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
// Configuration de l'ADC en 8 bits avec AVCC comme référence

void adc_init(void)
{
    /* Section 24.9 Register Description p.257
     * Configuration de la référence AVCC (registre ADMUX)
     * REFS0 = 1, REFS1 = 0 : AVCC avec condensateur externe sur AREF
     */
    ADMUX = (1 << REFS0);
    
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
     * Prescaler 128 : 16MHz/128 = 125kHz (optimal)
     * ADPS[2:0] = 111 pour prescaler 128
     */
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

}

uint8_t adc_read(uint8_t channel)
{
    /* Sélection du canal (Table 24-4. Input Channel Selections):
     *    ADC0 (PC0 où est connecté RV1)
     *    ADC1 (PC1 où est connecté R14 -> ADC_LDR)
     *    ADC2 (PC2 où est connecté R20 -> ADC_NTC)
     * Bits 3:0 – MUX[3:0]: Analog Channel Selection Bits (page 258) :
     *      MUX[3:0] = 0000 pour ADC0 
     *      MUX[3:0] = 0001 pour ADC1
     *      MUX[3:0] = 0010 pour ADC2
     *      .....
     *      MUX[3:0] = 1111 pour 0V (GND)
     */
    
    ADMUX = (ADMUX & 0b11110000) | (channel & 0b00001111);
    
    ADCSRA |= (1 << ADSC);
    
    while (ADCSRA & (1 << ADSC))
        ;
    return ADCH;
}
