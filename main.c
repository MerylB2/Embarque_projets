/*
 * Piscine embarquée - Module01: Timers
 * Exercices 02 et 03
 * 
 * Carte: 42Chips Devboard
 * Microcontrôleur: ATmega328P
 * F_CPU: 16MHz (quartz externe Y1)
 * 
 * Fichiers à rendre: Makefile, *.c, *.h
 * Fonctions autorisées: avr/io.h
 */

#define F_CPU 16000000UL  // 16MHz - Quartz externe (schéma Y1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
 * =============================================================================
 * EXERCICE 02: Rapport cyclique fixe
 * =============================================================================
 * 
 * Objectif: LED D2 clignote à 1 Hz avec rapport cyclique de 10%
 * Contraintes:
 * - Utiliser Timer1 (PB1) pour commander la LED
 * - Boucle infinie vide
 * - Ne pas utiliser PORTx directement
 * 
 * Pins selon schéma elec42_pool.pdf:
 * - LED_D2 connectée à PB1 (pin 13) - OC1A (Output Compare 1A)
 * - Résistance série 5.1K
 */

/*
 * Configuration du Timer1 en mode Fast PWM 8-bit
 * 
 * Principe: Génération d'un signal PWM à 1 Hz avec rapport cyclique de 10%
 * 
 * Références datasheet ATmega328P:
 * - Section 15: 16-bit Timer/Counter1 with PWM (p.91-120)
 * - Section 15.9.3: Fast PWM Mode (p.109)
 * - Table 15-5: Clock Select Bit Description (p.110)
 * - Table 15-2: Waveform Generation Mode (p.108)
 * 
 * Calculs:
 * Pour obtenir 1 Hz (période = 1s):
 * f_PWM = F_CPU / (N * (1 + TOP))
 * 1 Hz = 16MHz / (1024 * (1 + TOP))
 * TOP = (16MHz / (1024 * 1)) - 1 = 15624
 * 
 * Pour rapport cyclique de 10%:
 * OCR1A = TOP * 0.10 = 15624 * 0.10 = 1562
 */

#ifdef EXERCICE_02

void init_timer1_pwm_ex02(void)
{
    // Configuration de PB1 (OC1A) en sortie
    // Réf: Section 13.1 - Port B Data Direction Register (p.51)
    DDRB |= (1 << PB1);
    
    // Mode Fast PWM avec ICR1 comme TOP
    // WGM13:0 = 1110 (mode 14)
    // Réf: Table 15-5 - Waveform Generation Mode (p.108)
    TCCR1A |= (1 << WGM11);
    TCCR1A &= ~(1 << WGM10);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    
    // Clear OC1A on Compare Match, set at TOP (non-inverting mode)
    // COM1A1:0 = 10
    // Réf: Table 15-3 - Compare Output Mode (p.107)
    TCCR1A |= (1 << COM1A1);
    TCCR1A &= ~(1 << COM1A0);
    
    // Prescaler = 1024 (CS12:10 = 101)
    // Réf: Table 15-5 - Clock Select (p.110)
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TCCR1B &= ~(1 << CS11);
    
    // Valeur TOP pour 1 Hz
    // Réf: Section 15.11.1 - ICR1 (p.111)
    ICR1 = 15624;
    
    // Rapport cyclique de 10%
    // Réf: Section 15.11.2 - OCR1A (p.111)
    OCR1A = 1562;  // 15624 * 0.10
}

int main(void)
{
    init_timer1_pwm_ex02();
    
    // Boucle infinie vide (exigence de l'exercice)
    while (1)
    {
        // Ne rien faire - le Timer1 gère tout automatiquement
    }
    
    return 0;
}

#endif // EXERCICE_02

/*
 * =============================================================================
 * EXERCICE 03: Le cycle de la vie
 * =============================================================================
 * 
 * Objectif: LED D2 clignote à 1 Hz avec rapport cyclique variable (10% à 100%)
 * Contraintes:
 * - Utiliser Timer1 (PB1) pour commander la LED
 * - SW1 incrémente le rapport cyclique de 10%
 * - SW2 décrémente le rapport cyclique de 10%
 * - Delay autorisé uniquement pour débounce des boutons
 * 
 * Pins selon schéma:
 * - LED_D2: PB1 (pin 13) - OC1A
 * - SW1: PD2 (pin 32) - INT0 avec pull-up 10K
 * - SW2: PD3 (pin 1) - INT1 avec pull-up 10K
 */

#ifdef EXERCICE_03

// Variables globales pour le rapport cyclique
volatile uint8_t duty_cycle_percent = 10;  // Rapport cyclique en % (10 à 100)

/*
 * Initialisation du Timer1 en mode Fast PWM (identique ex02)
 */
void init_timer1_pwm_ex03(void)
{
    // Configuration de PB1 (OC1A) en sortie
    DDRB |= (1 << PB1);
    
    // Mode Fast PWM avec ICR1 comme TOP (mode 14)
    TCCR1A |= (1 << WGM11);
    TCCR1A &= ~(1 << WGM10);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    
    // Clear OC1A on Compare Match (non-inverting)
    TCCR1A |= (1 << COM1A1);
    TCCR1A &= ~(1 << COM1A0);
    
    // Prescaler = 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TCCR1B &= ~(1 << CS11);
    
    // TOP pour 1 Hz
    ICR1 = 15624;
    
    // Rapport cyclique initial de 10%
    OCR1A = (ICR1 * duty_cycle_percent) / 100;
}

/*
 * Initialisation des boutons
 * 
 * Réf: Section 13 - I/O Ports (p.50-73)
 */
void init_buttons(void)
{
    // Configuration PD2 et PD3 en entrée
    // Réf: Section 13.2.1 - Configuring the Pin (p.51)
    DDRD &= ~((1 << PD2) | (1 << PD3));
    
    // Activation des pull-up internes
    // Réf: Section 13.2.6 - Alternate Port Functions (p.55)
    // Note: Les boutons ont déjà des pull-up externes 10K (schéma)
    // mais on active aussi les internes pour plus de robustesse
    PORTD |= (1 << PD2) | (1 << PD3);
}

/*
 * Lecture bouton avec anti-rebond
 * Retourne 1 si bouton pressé (avec débounce)
 * 
 * Réf: Section 13.2.2 - Reading the Pin Value (p.52)
 */
uint8_t button_pressed(uint8_t pin)
{
    // Vérifier si bouton pressé (niveau LOW car pull-up)
    if (!(PIND & (1 << pin)))
    {
        _delay_ms(50);  // Débounce (autorisé selon consigne)
        
        // Vérifier à nouveau après débounce
        if (!(PIND & (1 << pin)))
        {
            // Attendre relâchement du bouton
            while (!(PIND & (1 << pin)));
            _delay_ms(50);  // Débounce au relâchement
            return 1;
        }
    }
    return 0;
}

/*
 * Mise à jour du rapport cyclique
 */
void update_duty_cycle(void)
{
    // Calcul de OCR1A selon le pourcentage
    // OCR1A = (ICR1 * duty_cycle_percent) / 100
    OCR1A = ((uint32_t)ICR1 * duty_cycle_percent) / 100;
}

int main(void)
{
    init_timer1_pwm_ex03();
    init_buttons();
    
    // Boucle principale
    while (1)
    {
        // SW1: Incrémenter le rapport cyclique de 10%
        if (button_pressed(PD2))
        {
            if (duty_cycle_percent < 100)
            {
                duty_cycle_percent += 10;
                update_duty_cycle();
            }
        }
        
        // SW2: Décrémenter le rapport cyclique de 10%
        if (button_pressed(PD3))
        {
            if (duty_cycle_percent > 10)
            {
                duty_cycle_percent -= 10;
                update_duty_cycle();
            }
        }
    }
    
    return 0;
}

#endif // EXERCICE_03
