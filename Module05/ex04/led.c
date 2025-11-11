/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   led.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 20:41:03 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 21:03:55 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/* Configuration des LEDs selon le schéma :
 * 
 * D1-D4 : LEDs simples (jauge digitale)
 * Configuration :
 * - D1 (LED 25%)  → PB0
 * - D2 (LED 50%)  → PB1  
 * - D3 (LED 75%)  → PB2
 * - D4 (LED 100%) → PB4
 * 
 * D5 : LED RGB
 * - RED   → PD5 (OC0B - PWM)
 * - GREEN → PD6 (OC0A - PWM)
 * - BLUE  → PD3 (OC2B - PWM)
 */

void led_init(void)
{
    /* Configuration des LEDs de jauge (D1-D4) en sortie */
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
    
    /* Éteindre toutes les LEDs au départ */
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
    
    /* Configuration des pins RGB (D5) en sortie */
    DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD3);
    
    /* Configuration PWM pour LED RGB
     * Timer 0 pour RED (PD6/OC0A) et GREEN (PD5/OC0B)
     * Timer 2 pour BLUE (PD3/OC2B)
     */
    
    // Timer 0 : Fast PWM mode
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS00);  // Prescaler 1 (62.5 kHz PWM)
    
    // Timer 2 : Fast PWM mode  
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS20);  // Prescaler 1
}


void wheel(uint8_t pos)
{
    pos = 255 - pos;
    
    if (pos < 85)
    {
        set_rgb(255 - pos * 3, 0, pos * 3);
    }
    else if (pos < 170)
    {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    }
    else
    {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}


void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    (r == 0) ? (DDRD &= ~(1 << PD5)) : (DDRD |= (1 << PD5));
    (g == 0) ? (DDRD &= ~(1 << PD6)) : (DDRD |= (1 << PD6));
    (b == 0) ? (DDRD &= ~(1 << PD3)) : (DDRD |= (1 << PD3));
    OCR0B = r;  // RED sur PD5 (OC0B)
    OCR0A = g;  // GREEN sur PD6 (OC0A)
    OCR2B = b;  // BLUE sur PD3 (OC2B)
}

/* Affiche la jauge digitale sur les LEDs D1-D4
 * selon la valeur ADC (0-1023)
 * 
 * D1 : 25%  → ADC >= 256  (1023 * 0.25) → PB0
 * D2 : 50%  → ADC >= 512  (1023 * 0.50) → PB1
 * D3 : 75%  → ADC >= 768  (1023 * 0.75) → PB2
 * D4 : 100% → ADC >= 1023 (1023 * 1.00) → PB4
 */
void led_set_var(uint16_t adc_value)
{
    // Éteindre toutes les LEDs d'abord
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
    
    // Allumer les LEDs selon le niveau
    if (adc_value >= 256)   // >= 25%
        PORTB |= (1 << PB0);
    
    if (adc_value >= 512)   // >= 50%
        PORTB |= (1 << PB1);
    
    if (adc_value >= 768)   // >= 75%
        PORTB |= (1 << PB2);
    
    if (adc_value >= 1020)  // >= 100% (avec marge)
        PORTB |= (1 << PB4);
}
