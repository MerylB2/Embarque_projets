/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:58:40 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 20:23:23 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

// https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
// p.215
// Constantes calculées depuis Table 23-2
#define TEMP_OFFSET_0C  320   // TOS : valeur ADC à 0°C (calculée)
#define TEMP_GAIN_x100  128   // k × 100 : 1.28 LSB/°C

void adc_init(void)
{
    /* Configuration de la référence interne 1.1V (Table 23-3 p.217)
     * REFS[1:0] = 11 : Internal 1.1V Voltage Reference
     * Nécessaire pour le capteur de température
     */
    ADMUX = (1 << REFS1) | (1 << REFS0);
    
    /* Activation de l'ADC (Bit 7 – ADEN: ADC Enable p.218)
     */
    ADCSRA = (1 << ADEN);
    
    /* Configuration du prescaler 128 (p.219)
     * 16MHz/128 = 125kHz (optimal pour ADC)
     */
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    /* Délai pour stabilisation de la référence interne */
    _delay_ms(2);

}

uint16_t adc_read_temp(void)
{
    /* Sélection du canal de température (Table 23-4, page 218)
     * MUX[3:0] = 1000 : Temperature Sensor (ADC8)
     */
    ADMUX = (ADMUX & 0xF0) | 0x08;  // Canal 8 = capteur température
    
    /* Attendre que le multiplexeur se stabilise
     * "The first conversion result may be less accurate"
     */
    _delay_us(100);
    
    /* Démarrer la conversion */
    ADCSRA |= (1 << ADSC);
    
    /* Attendre la fin de conversion */
    while (ADCSRA & (1 << ADSC));
    
    /* Lire le résultat 10 bits */
    return ADC;
}


// p.215
int16_t convert_to_celsius(void)
{
    uint16_t adc_value = adc_read_temp();
    
    /* Formule selon Table 23-2 Valeurs ADC typiques :
     * 
     * -40°C  → 0x010D (269 décimal)
     * +25°C  → 0x0160 (352 décimal)
     * +125°C → 0x01E0 (480 décimal)
     * 
     * Section 23.8 Temperature Measurement :
     * "The voltage sensitivity is approximately 1LSB/°C"
     * 
     * Calcul du gain k (entre 25°C et 125°C) :
     * k = (480 - 352) / (125 - 25)
     * k = 128 / 100
     * k = 1.28 LSB/°C
     * 
     * Calcul de TOS (offset à 0°C) :
     * TOS = ADC_25C - (25 × k)
     * TOS = 352 - (25 × 1.28)
     * TOS = 352 - 32
     * TOS = 320
     * 
     * Formule finale :
     * T = (ADC - TOS) / k
     * T = (ADC - 320) / 1.28
     * 
     * En arithmétique entière :
     * T = ((ADC - 320) × 100) / 128
     * 
     * NB :
     * Ces valeurs sont typiques et varient d'une puce à l'autre.
     * La datasheet mentionne (Section 23.8.1, page 215) :
     * "Calibration values determined during test are available 
     *  in the signature row"
     * 
     * Pour une précision optimale (±1°C), il faudrait :
     * 1. Lire TS_GAIN et TS_OFFSET depuis la signature row
     * 2. Utiliser la formule de calibration d'usine
     * 
     * Formule avec calibration d'usine :
     * T = (((ADCH << 8) + ADCL) - (273 + 100 - TS_OFFSET)) × 128 / TS_GAIN + 25
     * 
     * Pour cet exercice, nous utilisons les valeurs typiques.
     */
    
    int32_t temp = ((int32_t)(adc_value - TEMP_OFFSET_0C) * 100) / TEMP_GAIN_x100;
    
    return (int16_t)temp;
}
