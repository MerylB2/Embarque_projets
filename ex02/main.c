/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:56:53 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/10/30 17:57:07 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * ============================================================================
 * EXERCICE 02 - CLIGNOTEMENT LED AVEC PWM (1 Hz, 10% DUTY CYCLE)
 * ============================================================================
 * 
 * OBJECTIF:
 *   Faire clignoter une LED à 1 Hz (1 cycle par seconde) avec un duty cycle 
 *   de 10% en utilisant le Timer1 en mode PWM.
 * 
 * MATERIEL:
 *   - ATmega328P @ 16 MHz
 *   - LED D2 sur PB1 (OC1A, pin 13)  ← CORRECTION selon schéma
 *   - Résistance 5.1kΩ en série avec la LED
 * 
 * PRINCIPE:
 *   - Timer1 en mode Fast PWM (Mode 14: TOP = ICR1)
 *   - Prescaler 256 pour ralentir l'horloge
 *   - Période: 1 seconde → Fréquence: 1 Hz
 *   - Duty cycle: 10% → LED allumée 100ms, éteinte 900ms
 * 
 * REFERENCES DATASHEET:
 *   - Section 16: 16-bit Timer/Counter1 with PWM
 *   - Section 16.9.5: Fast PWM Mode
 *   - Table 16-5: Waveform Generation Mode Bit Description
 *   - Table 16-6: Clock Select Bit Description
 * 
 * ============================================================================
 */

#include <avr/io.h>       // Définitions des registres AVR
                          // Datasheet: Tous les registres I/O sont définis
                          // dans ce header (DDRB, PORTB, TCCR1A, etc.)

/*
 * ============================================================================
 * FONCTION: timer1_init
 * ============================================================================
 * Configure le Timer1 pour générer un signal PWM à 1 Hz, 50% duty cycle
 * 
 * MODE FAST PWM (Mode 14):
 *   - WGM13:12:11:10 = 1110 (bits répartis entre TCCR1A et TCCR1B)
 *   - TOP = ICR1 (valeur définissant la période)
 *   - OCR1A définit le moment où OC1A change d'état (duty cycle)
 *   - Comportement: Clear on Compare Match, Set at BOTTOM
 * 
 * CALCULS:
 *   Prescaler: 256
 *   f_timer = F_CPU / Prescaler = 16000000 / 256 = 62500 Hz
 *   Période_tick = 1 / 62500 = 16 µs
 *   
 *   Pour obtenir 1 Hz (T = 1 s):
 *     ICR1 = (F_CPU / (Prescaler × f_PWM)) - 1
 *     ICR1 = (16000000 / (256 × 1)) - 1
 *     ICR1 = 62499
 *   
 *   Pour 10% duty cycle:
 *     OCR1A = ICR1 × 0.10 = 62499 × 0.10 = 6249
 *     (On arrondit: 62500 × 0.10 - 1 = 6249)
 * 
 * REFERENCES:
 *   - Section 16.9.5: "In fast PWM mode, the counter is incremented until 
 *     the counter value matches either one of the fixed values 0x00FF, 
 *     0x01FF, or 0x03FF (WGM13:0 = 5, 6, or 7), the value in ICR1 
 *     (WGM13:0 = 14), or the value in OCR1A (WGM13:0 = 15)."
 *   - Table 16-5: Waveform Generation Mode Bit Description
 *   - Table 16-6: Clock Select Bit Description
 * ============================================================================
 */
void timer1_init(void)
{
    /*
     * Configuration de TCCR1A (Timer/Counter1 Control Register A)
     * ============================================================
     * 
     * Format du registre:
     *   Bit 7:6 - COM1A1:0 (Compare Output Mode for Channel A)
     *   Bit 5:4 - COM1B1:0 (Compare Output Mode for Channel B)
     *   Bit 3:2 - Réservés
     *   Bit 1:0 - WGM11:10 (Waveform Generation Mode, bits bas)
     * 
     * Notre configuration:
     *   COM1A1:0 = 10 (Clear OC1A on Compare Match, Set at BOTTOM)
     *   COM1B1:0 = 00 (OC1B déconnecté, non utilisé)
     *   WGM11:10 = 10 (partie basse du mode 14)
     * 
     * Résultat: 0b10000010 = 0x82
     */
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    
    // Datasheet Section 16.11.1, Table 16-3:
    // "COM1A1:0 = 10: Clear OC1A on Compare Match when up-counting. 
    //  Set OC1A at BOTTOM (non-inverting mode)."
    // 
    // Explication:
    //   - À TCNT1 = 0 (BOTTOM): OC1A passe à 1 (LED allumée)
    //   - Quand TCNT1 atteint OCR1A: OC1A passe à 0 (LED éteinte)
    //   - À TCNT1 = ICR1 (TOP): TCNT1 reset à 0
    
    /*
     * Configuration de TCCR1B (Timer/Counter1 Control Register B)
     * ============================================================
     * 
     * Format du registre:
     *   Bit 7   - ICNC1 (Input Capture Noise Canceler)
     *   Bit 6   - ICES1 (Input Capture Edge Select)
     *   Bit 5   - Réservé
     *   Bit 4:3 - WGM13:12 (Waveform Generation Mode, bits hauts)
     *   Bit 2:0 - CS12:10 (Clock Select)
     * 
     * Notre configuration:
     *   ICNC1 = 0 (désactivé)
     *   ICES1 = 0 (non utilisé)
     *   WGM13:12 = 11 (partie haute du mode 14)
     *   CS12:10 = 100 (clk/256, prescaler = 256)
     * 
     * Mode complet: WGM13:12:11:10 = 1110 = Mode 14
     * 
     * Résultat: 0b00011100 = 0x1C
     */
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12);
    
    // Datasheet Section 16.11.2:
    // Table 16-5: "WGM13:0 = 14: Fast PWM, TOP = ICR1"
    // Table 16-6: "CS12:0 = 100: clkI/O/256 (From prescaler)"
    
    /*
     * Configuration de ICR1 (Input Capture Register 1)
     * =================================================
     * 
     * En mode Fast PWM 14, ICR1 définit la valeur TOP:
     *   - Le compteur TCNT1 compte de 0 à ICR1
     *   - Quand TCNT1 = ICR1, le compteur revient à 0
     *   - Cela définit la période du signal PWM
     * 
     * ICR1 = 62499 pour obtenir 1 Hz:
     *   Période = (ICR1 + 1) × (256 / 16000000)
     *   Période = 62500 × 16µs = 1 seconde
     */
    ICR1 = 62499;
    
    // Datasheet Section 16.11.6:
    // "The Input Capture Register (ICR1) is a 16-bit read/write register.
    //  [...] When the ICR1 is used as TOP value [...] it defines the 
    //  counter TOP value in the Waveform Generation modes."
    
    /*
     * Configuration de OCR1A (Output Compare Register 1 A)
     * =====================================================
     * 
     * OCR1A définit le point de changement du signal OC1A:
     *   - Quand TCNT1 < OCR1A: OC1A = HIGH (LED allumée)
     *   - Quand TCNT1 >= OCR1A: OC1A = LOW (LED éteinte)
     * 
     * OCR1A = 6249 pour obtenir 10% duty cycle:
     *   Duty% = (OCR1A + 1) / (ICR1 + 1) × 100
     *   Duty% = 6250 / 62500 × 100 = 10%
     * 
     * Temps ON:  6250 × 16µs = 100 ms
     * Temps OFF: 56250 × 16µs = 900 ms
     * Total:     1000 ms = 1 s ✓
     */
    OCR1A = 6249;
    
    // Datasheet Section 16.11.4:
    // "The Output Compare Registers contain a 16-bit value that is 
    //  continuously compared with the counter value (TCNT1). A match 
    //  can be used to generate an Output Compare interrupt, or to 
    //  generate a waveform output on the OC1x pin."
}

/*
 * ============================================================================
 * FONCTION: gpio_init
 * ============================================================================
 * Configure le pin PB1 (OC1A) comme sortie pour le signal PWM
 * 
 * PB1 est connecté à la LED D2 via une résistance de 5.1kΩ (R41)
 * 
 * REFERENCES:
 *   - Section 14.2.1: Configuring the Pin (DDRx)
 *   - Section 14.3.1: Alternate Port Functions
 * ============================================================================
 */
void gpio_init(void)
{
    /*
     * Configuration de DDRB (Data Direction Register B)
     * ==================================================
     * 
     * DDRB contrôle la direction des pins du PORTB:
     *   - 0 = Entrée (input)
     *   - 1 = Sortie (output)
     * 
     * PB1 (bit 1 de DDRB) doit être configuré en sortie pour que
     * le Timer1 puisse contrôler la LED via le signal OC1A.
     * 
     * DDB1 est une macro définie dans avr/io.h qui vaut 1.
     */
    DDRB |= (1 << DDB1);
    
    // Datasheet Section 14.2.1:
    // "The DDxn bit in the DDRx Register selects the direction of this pin.
    //  If DDxn is written logic one, Pxn is configured as an output pin.
    //  If DDxn is written logic zero, Pxn is configured as an input pin."
    
    // Datasheet Section 14.3.1:
    // "When the OC1A pin is enabled as an output, and COM1A1:0 bits are 
    //  written to one of the non-zero combinations, the pin will toggle 
    //  according to a special case of the Output Compare match."
    
    /*
     * Note: Pas besoin de configurer PORTB pour OC1A
     * ===============================================
     * 
     * En mode PWM, le Timer1 contrôle directement le pin OC1A.
     * PORTB est ignoré tant que le Timer est actif.
     * 
     * Si on voulait désactiver le PWM, on pourrait:
     *   - Mettre TCCR1A = 0 (désactive le Timer)
     *   - Utiliser PORTB |= (1 << PB1) pour allumer manuellement
     *   - Utiliser PORTB &= ~(1 << PB1) pour éteindre manuellement
     */
}

/*
 * ============================================================================
 * FONCTION: main
 * ============================================================================
 * Programme principal
 * 
 * FONCTIONNEMENT:
 *   1. Initialise le GPIO (PB1 en sortie)
 *   2. Initialise le Timer1 (PWM à 1 Hz, 50% duty)
 *   3. Entre dans une boucle infinie (le Timer gère tout automatiquement)
 * 
 * AVANTAGE DU HARDWARE PWM:
 *   Le Timer1 génère le signal PWM de manière totalement autonome.
 *   Le CPU n'a rien à faire après l'initialisation !
 *   Pas besoin de boucle avec delay, pas d'interruptions, rien.
 * ============================================================================
 */
int main(void)
{
    /*
     * Étape 1: Initialisation du GPIO
     * ================================
     * Configure PB1 (OC1A) comme sortie pour que le Timer puisse 
     * contrôler la LED.
     */
    gpio_init();
    
    /*
     * Étape 2: Initialisation du Timer1
     * ==================================
     * Configure le Timer en mode Fast PWM:
     *   - Période: 1 Hz (1 cycle par seconde)
     *   - Duty cycle: 10% (100 ms ON, 900 ms OFF)
     */
    timer1_init();
    
    /*
     * Étape 3: Boucle infinie
     * ========================
     * 
     * Le programme n'a plus rien à faire !
     * 
     * Le hardware du Timer1 s'occupe de tout:
     *   - TCNT1 s'incrémente automatiquement à chaque tick (16µs)
     *   - Quand TCNT1 = 0: OC1A = HIGH → LED allumée
     *   - Quand TCNT1 = OCR1A (31249): OC1A = LOW → LED éteinte
     *   - Quand TCNT1 = ICR1 (62499): TCNT1 reset à 0
     *   - Et ça recommence indéfiniment...
     * 
     * Aucune intervention du CPU n'est nécessaire !
     * Le CPU pourrait même être mis en veille (sleep mode) pour 
     * économiser de l'énergie.
     */
    while(1)
    {
        // Rien à faire ici !
        // On pourrait mettre le CPU en veille:
        // set_sleep_mode(SLEEP_MODE_IDLE);
        // sleep_mode();
    }
    
    return 0;  // Jamais atteint (boucle infinie)
}

/*
 * ============================================================================
 * NOTES ET EXPLICATIONS COMPLEMENTAIRES
 * ============================================================================
 * 
 * 1. POURQUOI LE PRESCALER 256 ?
 *    --------------------------------
 *    Sans prescaler (clk/1), le Timer compte à 16 MHz:
 *      - Période max = 65536 ticks / 16000000 Hz = 4.096 ms
 *    
 *    Avec prescaler 256, le Timer compte à 62.5 kHz:
 *      - Période max = 65536 ticks / 62500 Hz = 1.048 s
 *    
 *    Le prescaler permet d'atteindre des périodes longues (secondes)
 *    avec un compteur 16-bit !
 * 
 * 
 * 2. POURQUOI LE MODE FAST PWM 14 ?
 *    ---------------------------------
 *    Il existe plusieurs modes PWM dans le Timer1:
 *      - Mode 14: Fast PWM avec TOP = ICR1
 *      - Mode 15: Fast PWM avec TOP = OCR1A
 *    
 *    Mode 14 est préférable car:
 *      - ICR1 définit la période (TOP)
 *      - OCR1A et OCR1B définissent les duty cycles
 *      - On peut avoir 2 canaux PWM indépendants !
 *    
 *    Mode 15 ne permet qu'un seul canal (OC1B) car OCR1A sert de TOP.
 * 
 * 
 * 3. CALCUL DE LA FREQUENCE PWM
 *    ---------------------------
 *    Formule générale:
 *      f_PWM = F_CPU / (Prescaler × (1 + TOP))
 *    
 *    Dans notre cas:
 *      f_PWM = 16000000 / (256 × (1 + 62499))
 *      f_PWM = 16000000 / (256 × 62500)
 *      f_PWM = 16000000 / 16000000
 *      f_PWM = 1 Hz ✓
 * 
 * 
 * 4. CALCUL DU DUTY CYCLE
 *    ----------------------
 *    Formule:
 *      Duty% = (OCR1A + 1) / (ICR1 + 1) × 100
 *    
 *    Dans notre cas:
 *      Duty% = (6249 + 1) / (62499 + 1) × 100
 *      Duty% = 6250 / 62500 × 100
 *      Duty% = 0.1 × 100
 *      Duty% = 10% ✓
 * 
 * 
 * 5. VERIFICATION SELON LE SCHEMA
 *    -----------------------------
 *    Selon le schéma elec42_pool.pdf:
 *      - LED D2 est connectée à PB1 (OC1A, pin 13)
 *      - Résistance R41 = 5.1kΩ en série
 *      - Anode de D2 → R41 → PB1 → Cathode de D2 → GND
 * 
 * 
 * 6. AVANTAGES DU HARDWARE PWM
 *    --------------------------
 *    vs. software PWM (avec delay):
 *      ✓ Précision parfaite (pas de jitter)
 *      ✓ CPU libre pour autres tâches
 *      ✓ Consommation énergétique réduite
 *      ✓ Pas de risque de blocage
 *      ✓ Plusieurs canaux simultanés (OC1A + OC1B)
 * 
 * 
 * 7. RESOLUTION DU PWM
 *    ------------------
 *    Avec ICR1 = 62499:
 *      - Résolution = ICR1 + 1 = 62500 valeurs possibles
 *      - Précision = 1 / 62500 = 0.0016% = 16 ppm
 *    
 *    Incrémenter OCR1A de 1 change le duty de 0.0016% !
 * 
 * 
 * 8. EXTENSIONS POSSIBLES
 *    ---------------------
 *    - Ajouter OC1B (PB2) pour contrôler une 2ème LED
 *    - Utiliser les interruptions (TIMER1_COMPA_vect) pour des actions
 *    - Varier le duty cycle pour un effet de fade in/out
 *    - Utiliser Phase Correct PWM pour un signal plus symétrique
 * 
 * ============================================================================
 */