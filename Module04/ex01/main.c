/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:48:13 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/06 15:49:48 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
 * Configuration matérielle (voir schéma elec42_pool.pdf) :
 * - LED D2 -> PB1 (OC1A - Timer1 Output Compare A)
 * 
 * Timer0 : Génère une interruption périodique pour MAJ le duty cycle
 * Timer1 : Génère le signal PWM sur PB1 (mode Fast PWM 8-bit)
 */

// Variables globales pour la gestion du PWM
volatile uint8_t pwm_val = 0;        // Valeur actuelle du duty cycle (0-255)
volatile int8_t pwm_dir = 1;         // Direction : 1 = montée, -1 = descente
volatile uint16_t timer_counter = 0; // Compteur pour temporisation

void init_timer1_pwm(void)
{
	DDRB |= (1 << PB1);
	
	/*
	 * Configuration Timer1 en mode Fast PWM 8-bit
	 * Mode 5 : Fast PWM, 8-bit
	 * WGM13 WGM12 WGM11 WGM10
	 *   0     1     0     1
	 * TOP = 0x00FF (255)
	 */
	TCCR1A |= (1 << WGM10);    // WGM10 = 1
	TCCR1B |= (1 << WGM12);    // WGM12 = 1
	
	/*
	 * Configuration du mode de sortie PWM sur OC1A(PB1)
	 * 
	 * COM1A1 COM1A0 | Description
	 *   1      0    | Clear OC1A on compare match, set at BOTTOM (non-inverting)
	 */
	TCCR1A |= (1 << COM1A1);
	
	/*
	 * Sélection du prescaler pour Timer1
	 * Pour une fréquence PWM élevée (pas de clignotement visible):
	 * Prescaler = 64
	 * Fréquence PWM = F_CPU / (prescaler * 256) = 16MHz / (64 * 256) ≈ 977 Hz
	 * 
	 * CS12 CS11 CS10
	 *  0    1    1   = clk/64
	 */
	TCCR1B |= (1 << CS11) | (1 << CS10);
	
	// Initialise le duty cycle à 0%
	// OCR1A définit le point de comparaison
	OCR1AL = 0;
}

void init_timer0_interrupt(void)
{
	/*
	 * Configuration Timer0 en mode CTC
	 * 
	 * Mode 2 : CTC
	 * WGM02 WGM01 WGM00
	 *   0     1     0
	 * TOP = OCRA
	 */
	TCCR0A |= (1 << WGM01);
	
	/*
	 * Sélection du prescaler pour Timer0
	 * Pour une interruption à ~1ms :
	 * Prescaler = 64
	 * OCR0A = (F_CPU / (prescaler * fréquence_souhaitée)) - 1
	 * OCR0A = (16000000 / (64 * 1000)) - 1 = 249
	 * 
	 * CS02 CS01 CS00
	 *  0    1    1   = clk/64
	 */
	TCCR0B |= (1 << CS01) | (1 << CS00);
	
	// Définit la valeur de comparaison pour ~1ms
	// OCR0A définit le TOP en mode CTC
	OCR0A = 249;
	
	/*
	 * Active l'interruption Timer0 Compare Match A
     * TIMSK0 (Timer/Counter Interrupt Mask Register)
	 * OCIE0A : Timer/Counter0 Output Compare Match A Interrupt Enable
	 */
	TIMSK0 |= (1 << OCIE0A);
	
	// Active les interruptions globales
	sei();
}

void update_pwm_duty()
{
	/*
	 * Calcul du nombre d'incréments nécessaires :
	 * - Cycle complet : 0 -> 255 -> 0 = 510 pas
	 * - Durée totale : 1000 ms
	 * - Intervalle entre chaque appel : ~1 ms
	 * - Incrément à chaque appel : 510 / 1000 ≈ 0.51
	 * 
	 * Pour simplifier et obtenir exactement 1 sec :
	 * On incrémente tous les ~2ms (500 incréments * 2ms = 1000ms)
	 */
	
	static uint8_t count = 0;
	
	// Ne met à jour que tous les 2 appels (~2ms)
	count++;
	if (count < 2)
		return;
	count = 0;
	
	// Met à jour la valeur PWM
	pwm_val += pwm_dir;
	
	// Change de direction aux extrémités
	if (pwm_val >= 255)
	{
		pwm_val = 255;
		pwm_dir = -1;
	}
	else if (pwm_val == 0 && pwm_dir == -1)
	{
		pwm_val = 0;
		pwm_dir = 1;
	}
	
	// Applique la nouvelle valeur au registre PWM
	// OCR1AL pour la partie basse en mode 8-bit
	OCR1AL = pwm_val;
}

/*
 * Vecteur d'interruption Timer0 Compare Match A
 * Vecteur 14 - TIMER0_COMPA
 * voir isr.h
 */
ISR_TC0_COMPARE_MATCH_A
{
	update_pwm_duty();
}

int main(void)
{
	init_timer1_pwm();
	init_timer0_interrupt();

	while (1)
	{}
}
