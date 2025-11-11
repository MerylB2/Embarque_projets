/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 18:19:27 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 17:19:47 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

void adc_init(void)
{
    /* Section 24.9 Register Description p.257
     * Configuration de la référence AVCC (registre ADMUX)
     * REFS0 = 1, REFS1 = 0 : AVCC avec condensateur externe sur AREF
     */
    ADMUX = (1 << REFS0);
    
    /* Activation de l'ADC (page 258, registre ADCSRA)
     * Bit 7 – ADEN: ADC Enable
     * ADEN = 1 : Active l'ADC
     */
    ADCSRA = (1 << ADEN);  // D'ABORD activer l'ADC
    
    /* Configuration du prescaler (page 259)
     * Prescaler 128 : 16MHz/128 = 125kHz (optimal)
     * ADPS[2:0] = 111 pour prescaler 128
     */
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // PUIS ajouter le prescaler avec |=
    
    /* Première conversion dummy (page 258)
     * La première conversion après activation doit être jetée
     */
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));

}

uint16_t adc_read(uint8_t channel)
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
    
    // Lecture 10 bits : ADCL puis ADCH (ordre obligatoire, page 259)
    return ADC;
}
