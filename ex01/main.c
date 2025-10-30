/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:16:32 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/10/30 13:03:57 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * Clignotement LED avec Timer1
 * LED D2 sur PB1 - Fréquence: 1 Hz (500ms ON / 500ms OFF)
 * CPU: ATmega328P - 16MHz
 * page 120 a 148 chap 16  et 17 du DataSheet complet
 */


int main(void)
{
    // Section 14.4.3: DDRB Register
    // PB1 doit être en sortie pour que OC1A fonctionne
    DDRB |= (1 << DDB1);
    
    /**  Configuration Timer1 Mode CTC (Clear Timer Compare) **/ 
    // TCCR1A : Compare Output Mode + Waveform Generation
    // Section 16.11.1: TCCR1A – Timer/Counter1 Control Register A
    //
    // Table 16-3: Compare Output Mode, Non-PWM Mode
    // COM1A1:0 = 01 → Toggle OC1A on Compare Match
    //
    // Table 16-5: Waveform Generation Mode Bit Description
    // WGM13:10 = 0100 → Mode 4, CTC (Clear Timer on Compare Match)
    
    TCCR1A = (1 << COM1A0);
    //         └─> Bit COM1A0 = 1 : Toggle OC1A automatiquement
    
    
    // TCCR1B : Waveform Mode + Clock Select
    // Section 16.11.2: TCCR1B – Timer/Counter1 Control Register B
    //
    // Table 16-5: WGM12 = 1 (partie du mode CTC 4)
    //
    // Table 16-6: Clock Select Bit Description
    // CS12:10 = 100 → clkI/O/256 (Prescaler = 256)
    
    TCCR1B = (1 << WGM12) | (1 << CS12);
    //         └────┬────┘   └────┬────┘
    //           CTC Mode     Prescaler 256
    
    
    /*** OCR1A : Valeur TOP pour mode CTC ***/
    // Section 16.11.4: OCR1AH and OCR1AL – Output Compare Register 1 A
    // Section 16.9.2: Clear Timer on Compare Match (CTC) Mode
    // OCR1A: Output Compare Register 1A définit la valeur TOP en mode CTC
    // Formule CTC (Section 16.9.2):
    // f_OC1A = f_clk_I/O / (2 × N × (1 + OCR1A))
    // 
    // Avec N = Prescaler = 256
    // Pour f = 1 Hz (LED clignote 1 fois par seconde):
    // Le toggle doit se faire à 2 Hz (toutes les 0.5s)
    //
    // Calcul:
    // 1 Hz = 16000000 / (2 × 256 × (1 + OCR1A))
    // OCR1A = (16000000 / (2 × 256 × 1)) - 1
    // OCR1A = 31250 - 1 = 31249
    
    OCR1A = 31249;
    //        └───────> Valeur TOP pour 1 Hz avec Prescaler 256
    
        
    /** Boucle infinie vide **/
    // Le hardware Timer1 gère le toggle de OC1A (PB1) automatiquement
    // Aucune intervention CPU nécessaire !
    // Le CPU est 100% disponible pour autres tâches
    
    while(1) {}
    
    return 0;
}
