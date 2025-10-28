
#include <avr/io.h>
// #include <util/delay.h>

// /* Ref chap 17 page 146 du data Sheet complet */
//  void init()
// {
// 	DDRB |= (1 << PB1);
// 	PORTB &= ~(1 << PB1);
// }

// /* avec le delay */
//  int main()
// {
// 	init();
// 	while (1)
// 	{
// 		PORTB ^= (1 << PB1);
// 		_delay_ms (5);
// 	}
// }

#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * Programme de clignotement LED avec Timer1
 * LED D2 sur PB1 - Fréquence: 1 Hz (500ms ON / 500ms OFF)
 * CPU: ATmega328P @ 16MHz
 */

int main(void)
{
    /* Configuration de PB1 (LED D2) en sortie */
    DDRB |= (1 << DDB1);  // Data Direction Register B, bit 1 à 1 = sortie

    /*
     * Configuration du Timer1 en mode CTC (Clear Timer on Compare Match)
     * avec interruption sur Compare Match A
     */
    
    /* TCCR1A - Timer/Counter1 Control Register A
     * COM1A1:0 = 01 : Toggle OC1A (PB1) on Compare Match
     * COM1B1:0 = 00 : OC1B déconnecté
     * WGM11:0 = 00 : Mode CTC (partie basse, complété par TCCR1B)
     */
    TCCR1A = (1 << COM1A0);

    /* TCCR1B - Timer/Counter1 Control Register B
     * WGM13:2 = 01 : Mode CTC (TOP = OCR1A), combiné avec TCCR1A
     * CS12:0 = 100 : Prescaler = 256
     * Fréquence timer = 16MHz / 256 = 62500 Hz
     */
    TCCR1B = (1 << WGM12) | (1 << CS12);

    /*
     * OCR1A - Output Compare Register 1 A
     * Valeur de comparaison pour basculer la LED
     * Pour une période de 1 Hz (toggle tous les 0.5s):
     * OCR1A = (F_CPU / Prescaler / (2 * Fréquence_désirée)) - 1
     * OCR1A = (16000000 / 256 / 2) - 1 = 31249
     * 
     * Le Timer1 compte de 0 à OCR1A, puis remet à 0 et toggle PB1
     * Période de toggle = 0.5s → Fréquence LED = 1 Hz
     */
    OCR1A = 31249;

    /* TIMSK1 - Timer/Counter1 Interrupt Mask Register
     * OCIE1A = 0 : Pas d'interruption nécessaire (mode hardware toggle)
     * Le mode COM1A0 gère automatiquement le toggle de PB1
     */
    // Pas besoin d'activer les interruptions, le hardware toggle fonctionne seul

    /* Boucle infinie vide - Le Timer1 gère tout automatiquement */
    while (1)
    {
        /* Rien à faire ici - Le Timer1 toggle automatiquement PB1 */
    }

    return 0;
}