/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 20:45:08 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 20:47:44 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


int main(void)
{
    uint16_t adc_value;
    uint8_t wheel_position;
    
    // Initialisation
    adc_init();
    led_init();
    
    while (1)
    {
        // Lecture du potentiomètre RV1 (ADC0)
        adc_value = adc_read(0);
        
        // 1. Mise à jour (LEDs D1-D4)
        led_set_var(adc_value);
        
        // 2. Conversion ADC (0-1023) vers position wheel (0-255)
        wheel_position = adc_value >> 2;  // Division par 4 : 1023/4 = 255
        
        // 3. Génération et application de la couleur via fonction wheel
        wheel(wheel_position);
        
        // Petit délai pour stabilité
        _delay_ms(10);
    }
}