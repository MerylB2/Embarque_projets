#ifndef ISR_H
#define ISR_H

// https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
// https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html

// #define ISR(n) __attribute__((signal))void __vector_##n()
//    │    │   │         │                         │    ││  │
//    │    │   │         │                         │    ││  └─ Parenthèses de fonction
//    │    │   │         │                         │    │└─ Valeur du paramètre collée
//    │    │   │         │                         │    └─ Token pasting operator
//    │    │   │         │                         └─ Nom de base du vecteur
//    │    │   │         └─ Attribut GCC pour ISR (RETI + sauvegarde contexte)
//    │    │   └─ Paramètre (numéro du vecteur)
//    │    └─ Nom de la macro
//    └─ Directive préprocesseur

/* __attribute__((signal)) : attribut GCC (spécifique au compilateur avr-gcc)
 * Indique au compilateur que cette fonction est une routine d'interruption
 * Le compilateur va :
 *  - Sauvegarder automatiquement tous les registres au début
 *  - Restaurer tous les registres à la fin
 *  - Utiliser l'instruction RETI au lieu de RET (Return from Interrupt)
 *  - Désactiver l'optimisation qui pourrait casser le contexte d'interruption
*/


/* void __vector_##n() : nom de la fonction avec le token pasting operator ##
 * opérateur ## (token pasting): colle 2 tokens ensemble pendant la préprocession.
 * Exple :
 *      #define ISR_EXTERNAL_0  ISR(1)
 *          // Étape 1 : Remplace ISR_EXTERNAL_0 par ISR(1)
            ISR_EXTERNAL_0
                ↓
            ISR(1)

            // Étape 2 : Remplace n par 1 dans la macro ISR
            #define ISR(n) __attribute__((signal)) void __vector_##n()
                                                                ↓
                        __attribute__((signal)) void __vector_##1()

            // Étape 3 : Token pasting (## colle __vector_ et 1)
                        __attribute__((signal)) void __vector_1()
*/

#define ISR(n) __attribute__((signal)) void __vector_##n()

#define ISR_RESET                       ISR(0)
#define ISR_EXTERNAL_0                  ISR(1)
#define ISR_EXTERNAL_1                  ISR(2)
#define ISR_PIN_CHANGE_0                ISR(3)
#define ISR_PIN_CHANGE_1                ISR(4)
#define ISR_PIN_CHANGE_2                ISR(5)
#define ISR_WATCHDOG_TIMEOUT            ISR(6)
#define ISR_TC2_COMPARE_MATCH_A         ISR(7)
#define ISR_TC2_COMPARE_MATCH_B         ISR(8)
#define ISR_TC2_OVERFLOW                ISR(9)
#define ISR_TC1_CAPTURE                 ISR(10)
#define ISR_TC1_COMPARE_MATCH_A         ISR(11)
#define ISR_TC1_COMPARE_MATCH_B         ISR(12)
#define ISR_TC1_OVERFLOW                ISR(13)
#define ISR_TC0_COMPARE_MATCH_A         ISR(14)
#define ISR_TC0_COMPARE_MATCH_B         ISR(15)
#define ISR_TC0_OVERFLOW                ISR(16)
#define ISR_SPI_TRANSFER_COMPLETE       ISR(17)
#define ISR_USART_RECEPTION_COMPLETE    ISR(18)
#define ISR_USART_DATA_REGISTER_EMPTY   ISR(19)
#define ISR_USART_TRANSMISSION_COMPLETE ISR(20)
#define ISR_ADC_CONVERSION_COMPLETE     ISR(21)
#define ISR_EEPROM_READY                ISR(22)
#define ISR_ANALOG_COMPARATOR           ISR(23)
#define ISR_TWI                         ISR(24)
#define ISR_SPM_READY                   ISR(25)

#endif