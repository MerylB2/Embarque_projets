/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:58:40 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 20:29:21 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "uart.h"

// // Constantes calculées depuis Table 24-2
// #define TEMP_OFFSET_0C  266   // TOS : valeur ADC à 0°C (calculée)
// #define TEMP_GAIN_x100  102   // k × 100 : 1.024 LSB/°C

// /* Section 24.8 Temperature Measurement (page 256-257)
//  * Canal ADC8 (MUX[3:0] = 1000, page 258, Table 24-4)
//  * Tension de référence interne 1.1V (REFS[1:0] = 11)
//  * Offset : ~314mV à 25°C
//  * Gain : ~1mV/°C*/

// void adc_init(void)
// {
//     /* Configuration de la référence interne 1.1V (Table 24-3 p.257)
//      * REFS[1:0] = 11 : Internal 1.1V Voltage Reference
//      * Nécessaire pour le capteur de température
//      */
//     ADMUX = (1 << REFS1) | (1 << REFS0);
    
//     /* Activation de l'ADC (Bit 7 – ADEN: ADC Enable p.258)
//      */
//     ADCSRA = (1 << ADEN);
    
//     /* Configuration du prescaler 128 (p.259)
//      * 16MHz/128 = 125kHz (optimal pour ADC)
//      */
//     ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
//     /* Délai pour stabilisation de la référence interne */
//     _delay_ms(2);
    
// }

// uint16_t adc_read_temp(void)
// {
//     /* Sélection du canal de température (Table 24-4, page 258)
//      * MUX[3:0] = 1000 : Temperature Sensor (ADC8)
//      */
//     ADMUX = (ADMUX & 0xF0) | 0x08;  // Canal 8 = capteur température
    
//     /* Attendre que le multiplexeur se stabilise */
//     _delay_us(100);
    
//     /* Démarrer la conversion */
//     ADCSRA |= (1 << ADSC);
    
//     /* Attendre la fin de conversion */
//     while (ADCSRA & (1 << ADSC));
    
//     /* Lire le résultat 10 bits */
//     return ADC;
// }


// int16_t covert_to_celsius(void)
// {
//     uint16_t adc_value = adc_read_temp_raw();
    
//     /* Formule officielle (Section 24.8, page 256) :
//      * T = { [(ADCH << 8) | ADCL] - TOS } / k
//      * 
//      * Où :
//      * - TOS = Temperature Offset Sensor (valeur ADC à 0°C)
//      * - k = Gain (coefficient de température en LSB/°C)
//      * 
//      * D'après Table 24-2 (valeurs typiques) :
//      * - À -45°C : 242 mV
//      * - À +25°C : 314 mV  
//      * - À +85°C : 380 mV
//      * 
//      * Calcul du gain k :
//      * k (mV/°C) = (380 - 314) / (85 - 25) = 66 / 60 = 1.1 mV/°C
//      * 
//      * Avec Vref = 1.1V :
//      * - 1 LSB = 1100 mV / 1024 = 1.074 mV
//      * - k (LSB/°C) = 1.1 / 1.074 = 1.024 ≈ 1.0 LSB/°C
//      * 
//      * Calcul de TOS (offset à 0°C) :
//      * - ADC à 25°C = (314 mV / 1100 mV) × 1024 = 292
//      * - TOS = 292 - (25 × 1.024) = 292 - 25.6 = 266
//      * 
//      * Formule finale :
//      * T = (ADC - 266) / 1.024
//      * 
//      * En arithmétique entière (éviter float) :
//      * T = ((ADC - 266) × 100) / 102
//      * 
//      * Note : Ces valeurs sont typiques et varient d'une puce à l'autre.
//      * TOS devrait idéalement être lu depuis l'EEPROM (stocké en usine)
//      * mais cette fonctionnalité n'est pas documentée pour l'ATmega328P.
//      * Pour une meilleure précision, calibrer à 2 températures connues.
//      */
    
//     int32_t temp = ((int32_t)(adc_value - TEMP_OFFSET_0C) * 100) / TEMP_GAIN_x100;
    
//     return (int16_t)temp;
// }


/* NB:
 * Table 23-2 (ATmega32p datasheet 294p) meilleur
 *      Donne des valeurs ADC directes (0x010D, 0x0160, 0x01E0)
 *      Plus grande plage (-40°C à +125°C)
 *      Plus précise (valeurs mesurées en production)

 * Table 24-2 (datasheet 662p)
 *      Donne des tensions (242mV, 314mV, 380mV)
 *      Plus petite plage (-45°C à +85°C)
 *      Nécessite conversion tension → ADC (source d'erreur)
 */