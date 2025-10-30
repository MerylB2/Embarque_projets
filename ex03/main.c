/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:44:07 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/10/30 17:44:41 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * ============================================================================
 * EXERCICE 03 - CONTROLE DUTY CYCLE PAR BOUTONS
 * ============================================================================
 * 
 * OBJECTIF:
 *   Contrôler le duty cycle d'un signal PWM entre 10% et 100% avec deux 
 *   boutons (SW1 pour incrémenter, SW2 pour décrémenter).
 * 
 * MATERIEL:
 *   - ATmega328P @ 16 MHz
 *   - LED D2 sur PB1 (OC1A, pin 13)  ← CORRECTION
 *   - Bouton SW1 sur PD2 (INT0, pin 32) - Increment +10%
 *   - Bouton SW2 sur PD4 (XCK/T0, pin 2) - Decrement -10%  ← CORRECTION
 *   - Résistances pull-up 10kΩ sur boutons
 *   - Résistance 5.1kΩ pour LED D2
 * 
 * PRINCIPE:
 *   - Timer1 en mode Fast PWM (Mode 14)
 *   - Période fixe: 1 seconde (ICR1 = 62499)
 *   - Duty cycle variable: OCR1A (6249 à 62499)
 *   - Lecture des boutons dans la boucle principale
 *   - Anti-rebond logiciel: délai de 200ms
 * 
 * REFERENCES DATASHEET:
 *   - Section 14: I/O Ports (GPIO)
 *   - Section 16: 16-bit Timer/Counter1 with PWM
 *   - Section 16.9.5: Fast PWM Mode
 *   - Table 16-5: Waveform Generation Mode
 *   - Table 16-6: Clock Select
 * 
 * ============================================================================
 */

#include <avr/io.h>
#include <util/delay.h>

/*
 * ============================================================================
 * CONFIGURATION DES CONSTANTES
 * ============================================================================
 */

// Valeurs PWM pour différents duty cycles
// Formule: OCR1A = (Duty% / 100) * (ICR1 + 1) - 1
#define DUTY_10_PERCENT   6249      // 10% de 62500 - 1
#define DUTY_20_PERCENT   12499     // 20%
#define DUTY_30_PERCENT   18749     // 30%
#define DUTY_40_PERCENT   24999     // 40%
#define DUTY_50_PERCENT   31249     // 50%
#define DUTY_60_PERCENT   37499     // 60%
#define DUTY_70_PERCENT   43749     // 70%
#define DUTY_80_PERCENT   49999     // 80%
#define DUTY_90_PERCENT   56249     // 90%
#define DUTY_100_PERCENT  62499     // 100%

// Pas d'incrément/décrément (10%)
#define DUTY_STEP         6250

// Limites
#define DUTY_MIN          DUTY_10_PERCENT
#define DUTY_MAX          DUTY_100_PERCENT

// Période PWM
#define PWM_TOP           62499     // Pour 1 Hz @ 256 prescaler

// Délai anti-rebond
#define DEBOUNCE_DELAY_MS 200

/*
 * ============================================================================
 * FONCTION: gpio_init
 * ============================================================================
 * Configure les GPIO (pins d'entrée/sortie)
 * 
 * CONFIGURATIONS:
 *   - PB1 (OC1A) : Sortie pour signal PWM vers LED D2
 *   - PD2 (INT0) : Entrée avec pull-up pour bouton SW1 (increment)
 *   - PD4 (XCK)  : Entrée avec pull-up pour bouton SW2 (decrement)
 * 
 * REFERENCES:
 *   - Section 14.2.1: DDRx (Data Direction Register)
 *   - Section 14.2.2: PORTx (Port Data Register)
 * ============================================================================
 */
void gpio_init(void)
{
    /*
     * Configuration PORTB
     * -------------------
     * PB1 (pin 13) = OC1A = Sortie PWM vers LED D2
     * 
     * DDRx: 0 = Input, 1 = Output
     */
    DDRB |= (1 << DDB1);    // PB1 en sortie
    // Datasheet Section 14.2.1: "The DDxn bit in the DDRx Register 
    // selects the direction of this pin. If DDxn is written logic one, 
    // Pxn is configured as an output pin."
    
    /*
     * Configuration PORTD
     * -------------------
     * PD2 (pin 32) = INT0 = SW1 = Entrée avec pull-up
     * PD4 (pin 2)  = XCK  = SW2 = Entrée avec pull-up
     */
    DDRD &= ~(1 << DDD2);   // PD2 en entrée (SW1)
    DDRD &= ~(1 << DDD4);   // PD4 en entrée (SW2)
    // Datasheet Section 14.2.1: "If DDxn is written logic zero, 
    // Pxn is configured as an input pin."
    
    /*
     * Activation des résistances pull-up internes
     * 
     * PORTxn = 1 (quand DDxn = 0) active le pull-up (~20-50kΩ)
     * Cela tire le pin vers +5V quand le bouton n'est pas pressé
     */
    PORTD |= (1 << PORTD2); // Pull-up sur PD2 (SW1)
    PORTD |= (1 << PORTD4); // Pull-up sur PD4 (SW2)
    // Datasheet Section 14.2.2: "If PORTxn is written logic one when 
    // the pin is configured as an input pin, the pull-up resistor is 
    // activated."
}

/*
 * ============================================================================
 * FONCTION: timer1_init
 * ============================================================================
 * Configure le Timer1 en mode Fast PWM (Mode 14)
 * 
 * MODE FAST PWM 14:
 *   - WGM13:12:11:10 = 1110
 *   - TOP = ICR1 (période flexible)
 *   - OCR1A définit le point de changement (duty cycle)
 *   - OC1A: Clear on Match, Set at BOTTOM (non-inverting)
 * 
 * CONFIGURATION:
 *   - Fréquence: 1 Hz
 *   - Prescaler: 256
 *   - TOP (ICR1): 62499
 *   - Duty initial: 10% (OCR1A = 6249)
 * 
 * CALCULS:
 *   f_PWM = F_CPU / (Prescaler × (1 + TOP))
 *   1 Hz = 16000000 / (256 × (1 + 62499))
 *   1 Hz = 16000000 / 16000000 = 1 Hz ✓
 * 
 * REFERENCES:
 *   - Section 16.9.5: Fast PWM Mode
 *   - Section 16.11.1: TCCR1A
 *   - Section 16.11.2: TCCR1B
 *   - Section 16.11.6: ICR1
 *   - Table 16-5: Waveform Generation Mode Bit Description
 *   - Table 16-6: Clock Select Bit Description
 * ============================================================================
 */
void timer1_init(void)
{
    /*
     * TCCR1A - Timer/Counter1 Control Register A
     * ==========================================
     * 
     * Bits 7:6 (COM1A1:0) = 10 : Clear OC1A on Compare Match,
     *                             Set OC1A at BOTTOM (non-inverting PWM)
     * Bits 5:4 (COM1B1:0) = 00 : OC1B déconnecté (non utilisé)
     * Bits 3:2            = 00 : Réservés
     * Bits 1:0 (WGM11:10) = 10 : Partie basse du mode WGM
     * 
     * Résultat: 0b10000010 = 0x82
     */
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    // Datasheet Section 16.11.1, Table 16-3:
    // COM1A1:0 = 10 → "Clear OC1A on Compare Match when up-counting.
    //                  Set OC1A at BOTTOM (non-inverting mode)."
    
    /*
     * TCCR1B - Timer/Counter1 Control Register B
     * ==========================================
     * 
     * Bit  7 (ICNC1)      = 0 : Input Capture Noise Canceler désactivé
     * Bit  6 (ICES1)      = 0 : Input Capture Edge Select (non utilisé)
     * Bit  5              = 0 : Réservé
     * Bits 4:3 (WGM13:12) = 11 : Partie haute du mode WGM
     * Bits 2:0 (CS12:10)  = 100: Clock Select = clk/256 (prescaler)
     * 
     * Mode complet: WGM13:12:11:10 = 1110 = Mode 14 (Fast PWM, TOP=ICR1)
     * 
     * Résultat: 0b00011100 = 0x1C
     */
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12);
    // Datasheet Section 16.11.2:
    // Table 16-5: WGM = 1110 → "Fast PWM, TOP = ICR1"
    // Table 16-6: CS = 100 → "clk/256 (From prescaler)"
    
    /*
     * ICR1 - Input Capture Register 1
     * ================================
     * 
     * En mode Fast PWM 14, ICR1 définit la valeur TOP du compteur.
     * Quand TCNT1 atteint ICR1, il se réinitialise à 0.
     * 
     * ICR1 = 62499 → Période de 1 seconde
     * 
     * Calcul: T = (ICR1 + 1) × Prescaler / F_CPU
     *         T = 62500 × 256 / 16000000 = 1 s
     */
    ICR1 = PWM_TOP;
    // Datasheet Section 16.11.6: "The Input Capture Register (ICR1) 
    // is a 16-bit read/write register."
    
    /*
     * OCR1A - Output Compare Register 1 A
     * ====================================
     * 
     * Définit le point où OC1A change d'état (duty cycle).
     * 
     * Duty cycle initial: 10%
     * OCR1A = 6249 → 100 ms ON, 900 ms OFF
     * 
     * Calcul: Duty% = (OCR1A + 1) / (ICR1 + 1) × 100
     *         10% = 6250 / 62500 × 100 = 10% ✓
     */
    OCR1A = DUTY_MIN;
    // Datasheet Section 16.11.4: "The Output Compare Registers contain 
    // a 16-bit value that is continuously compared with the counter 
    // value (TCNT1)."
}

/*
 * ============================================================================
 * FONCTION: main
 * ============================================================================
 * Boucle principale du programme
 * 
 * FONCTIONNEMENT:
 *   1. Initialisation du hardware (GPIO + Timer1)
 *   2. Boucle infinie:
 *      - Lecture des boutons (polling)
 *      - Modification du duty cycle si bouton pressé
 *      - Anti-rebond logiciel (délai)
 * 
 * DETECTION BOUTON:
 *   Les boutons sont actifs BAS (pull-up):
 *     - Non pressé: PINx = 1 (HIGH, tiré vers +5V par pull-up)
 *     - Pressé:     PINx = 0 (LOW, relié à GND)
 * 
 * REFERENCES:
 *   - Section 14.2.3: PINx (Port Input Pins)
 * ============================================================================
 */
int main(void)
{
    /*
     * Variable pour stocker la valeur actuelle du duty cycle
     * Type uint16_t car OCR1A est un registre 16-bit (0-65535)
     */
    uint16_t duty = DUTY_MIN;   // Initialisation à 10%
    
    /*
     * Initialisation du système
     */
    gpio_init();                // Configure les pins
    timer1_init();              // Configure le Timer1
    
    /*
     * Boucle principale infinie
     * 
     * Le microcontrôleur exécute cette boucle en permanence.
     * Le hardware du Timer1 gère automatiquement le signal PWM.
     */
    while(1)
    {
        /*
         * ===================================================================
         * BOUTON SW1 (PD2) - INCREMENT (+10%)
         * ===================================================================
         * 
         * Lecture du pin PD2 via le registre PIND
         * 
         * PIND: Port Input Pins register D
         * Permet de lire l'état logique des pins du PORTD
         * 
         * Test: !(PIND & (1 << PIND2))
         *   - Si PD2 = HIGH (non pressé): (PIND & 0x04) = 0x04 → !(0x04) = 0 (faux)
         *   - Si PD2 = LOW (pressé):      (PIND & 0x04) = 0x00 → !(0x00) = 1 (vrai)
         */
        if (!(PIND & (1 << PIND2)))
        {
            // Datasheet Section 14.2.3: "Reading the PINx Register returns 
            // the value on the port pins. [...] The port pin can be read 
            // independently of the setting of Data Direction bit (DDxn)."
            
            /*
             * Incrémenter le duty cycle de 10%
             * 
             * On ajoute DUTY_STEP (6250) à la valeur actuelle.
             * Cela correspond à +10% car:
             *   6250 / 62500 = 0.1 = 10%
             */
            duty += DUTY_STEP;
            
            /*
             * Vérification de la limite maximum
             * 
             * Si duty > 62499 (100%), on limite à 62499.
             * Cela empêche un débordement (overflow) au-delà de 16 bits.
             */
            if (duty > DUTY_MAX)
            {
                duty = DUTY_MAX;    // Limite à 100%
            }
            
            /*
             * Mise à jour du registre OCR1A
             * 
             * Le hardware du Timer1 compare en permanence TCNT1 avec OCR1A.
             * Quand TCNT1 = OCR1A, le signal OC1A change d'état.
             * 
             * La modification prend effet immédiatement (au prochain cycle).
             */
            OCR1A = duty;
            // Datasheet Section 16.11.4: "The Output Compare Registers are 
            // 16-bit in size. [...] A write to the high byte (OCR1xH) blocks 
            // updates of the temporary register to the OCR1x until OCR1xL is 
            // written."
            
            /*
             * Délai anti-rebond
             * 
             * Attente de 200ms pour:
             *   1. Laisser le temps à l'utilisateur de relâcher le bouton
             *   2. Filtrer les rebonds mécaniques du bouton
             * 
             * Sans ce délai, une seule pression pourrait être détectée
             * plusieurs fois (typiquement 5-10 fois en 50-100ms).
             */
            _delay_ms(DEBOUNCE_DELAY_MS);
            // util/delay.h: Génère un délai précis basé sur F_CPU
        }
        
        /*
         * ===================================================================
         * BOUTON SW2 (PD4) - DECREMENT (-10%)
         * ===================================================================
         * 
         * Même principe que SW1, mais pour décrémenter le duty cycle.
         * 
         * Lecture du pin PD4 via PIND.
         */
        if (!(PIND & (1 << PIND4)))
        {
            /*
             * Décrémenter le duty cycle de 10%
             * 
             * ATTENTION: Risque d'underflow si duty < DUTY_STEP !
             * On doit vérifier AVANT de soustraire.
             */
            if (duty >= DUTY_MIN + DUTY_STEP)
            {
                // Soustraction sécurisée
                duty -= DUTY_STEP;
            }
            else
            {
                // Si on est déjà au minimum ou proche, forcer au minimum
                duty = DUTY_MIN;    // Limite à 10%
            }
            
            /*
             * Vérification supplémentaire de la limite minimum
             * 
             * Protection contre les erreurs de calcul ou underflow.
             */
            if (duty < DUTY_MIN)
            {
                duty = DUTY_MIN;
            }
            
            /*
             * Mise à jour du PWM
             */
            OCR1A = duty;
            
            /*
             * Anti-rebond
             */
            _delay_ms(DEBOUNCE_DELAY_MS);
        }
        
        /*
         * Pas d'autre traitement dans la boucle.
         * 
         * Le Timer1 gère automatiquement le signal PWM en hardware:
         *   - TCNT1 incrémente automatiquement à chaque tick
         *   - Quand TCNT1 = 0 (BOTTOM): OC1A = HIGH (LED ON)
         *   - Quand TCNT1 = OCR1A: OC1A = LOW (LED OFF)
         *   - Quand TCNT1 = ICR1 (TOP): TCNT1 reset à 0
         * 
         * Aucune intervention du CPU n'est nécessaire !
         */
    }
    
    return 0;   // Jamais atteint (boucle infinie)
}

/*
 * ============================================================================
 * NOTES ET EXPLICATIONS COMPLEMENTAIRES
 * ============================================================================
 * 
 * 1. POURQUOI LE PRESCALER 256 ?
 *    --------------------------------
 *    Sans prescaler, avec F_CPU = 16 MHz:
 *      - Période max = 65536 / 16000000 = 4.096 ms
 *    
 *    Avec prescaler 256:
 *      - f_timer = 16000000 / 256 = 62500 Hz
 *      - Période max = 65536 / 62500 = 1.048 s
 *    
 *    Cela permet d'atteindre 1 seconde avec un compteur 16-bit !
 * 
 * 
 * 2. POURQUOI MODE FAST PWM 14 (TOP = ICR1) ?
 *    -----------------------------------------
 *    Mode 15 (TOP = OCR1A) ne permet qu'un seul canal PWM (OC1B).
 *    Mode 14 (TOP = ICR1) libère OCR1A et OCR1B pour deux canaux PWM
 *    indépendants avec la même période.
 * 
 * 
 * 3. CALCUL DUTY CYCLE
 *    ------------------
 *    Duty% = (OCR1A + 1) / (ICR1 + 1) × 100
 *    
 *    Exemples:
 *      10%  : OCR1A = 6249  → (6250 / 62500) × 100 = 10%
 *      50%  : OCR1A = 31249 → (31250 / 62500) × 100 = 50%
 *      100% : OCR1A = 62499 → (62500 / 62500) × 100 = 100%
 * 
 * 
 * 4. TEMPS ON ET OFF
 *    ----------------
 *    T_ON  = (OCR1A + 1) × (256 / 16000000)
 *    T_OFF = (ICR1 - OCR1A) × (256 / 16000000)
 *    
 *    Pour 10% (OCR1A = 6249):
 *      T_ON  = 6250 × 16µs = 100 ms
 *      T_OFF = 56250 × 16µs = 900 ms
 *      Total = 1000 ms = 1 s ✓
 * 
 * 
 * 5. VERIFICATION DU SCHEMA
 *    -----------------------
 *    Selon le schéma elec42_pool.pdf:
 *      - LED D2 → PB1 (OC1A, pin 13) avec R41 (5.1kΩ)
 *      - SW1 → PD2 (INT0, pin 32) avec pull-up
 *      - SW2 → PD4 (XCK/T0, pin 2) avec pull-up
 * 
 * 
 * 6. ALTERNATIVE: INTERRUPTIONS
 *    ---------------------------
 *    Ce code utilise le polling (vérification en boucle).
 *    
 *    Une alternative serait d'utiliser INT0 pour SW1:
 *      - INT0 (PD2) déclenche automatiquement une ISR
 *      - Plus réactif et économe en énergie
 *      - Nécessite configuration EICRA et EIMSK
 * 
 * 
 * 7. OPTIMISATIONS POSSIBLES
 *    ------------------------
 *    - Ajouter un afficheur 7-segments pour montrer le duty%
 *    - Utiliser EEPROM pour sauvegarder la dernière valeur
 *    - Implémenter une rampe progressive (fade in/out)
 *    - Ajouter un mode automatique (cycle 10%-100%-10%)
 * 
 * ============================================================================
 */