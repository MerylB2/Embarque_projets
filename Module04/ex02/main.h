/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:48:45 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/06 16:12:50 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "isr.h"


#define SW1_PORT     PORTD
#define SW1_DDR      DDRD
#define SW1_PIN      PD2    // INT0

#define SW2_PORT     PORTD
#define SW2_DDR      DDRD
#define SW2_PIN      PD4    // INT0 sera configuré en PCINT

// Configuration des LEDs D1-D4
#define LED_D1       PB0    // Bit 0 du compteur
#define LED_D2       PB1    // Bit 1 du compteur
#define LED_D3       PB2    // Bit 2 du compteur
#define LED_D4       PB4    // Bit 3 du compteur

// Masque pour les LEDs
#define LED_MASK     ((1 << LED_D1) | (1 << LED_D2) | (1 << LED_D3) | (1 << LED_D4))

// Prototypes de fonctions
void config_leds(void);
void setup_sw_interrupt(void);
void update_leds(uint8_t value);

#endif