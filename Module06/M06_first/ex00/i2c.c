/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:06:53 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 15:09:25 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// datasheet : https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf


/* Module I2C (Inter-Integrated Circuit)
 * Soit TWI (Two Wire serial Interface) pour l'ATmega328p p.173
 * Section 22.2 2-wire Serial Interface Bus Definition :
 *      SDA (Serial Data): ligne de données bidirectionnelle
 *      SCL (Serial Clock) : ligne d'horloge générée par le maître
 *  
 * Architecture maître-esclave : l'ATmega328P est le maître qui initie les communications, 
 * les périphériques sont les esclaves avec des adresses uniques (7 ou 10 bits).
 * 
 * SCL & SDA pins :
 *  SDA -> PC4 (ACD4)
 *  SCL -> PC5 (ADC5)
 */


 /*
 * Génère une condition START sur le bus I2C
 *
 * Configure la fréquence I2C à 100kHz avec F_CPU = 16MHz
 * Formule: SCL_freq = F_CPU / (16 + 2*TWBR*Prescaler)
 * 
 * Pour 100kHz avec Prescaler=1 :
 * 100000 = 16000000 / (16 + 2*TWBR*1)
 * TWBR = 72
 * 
 * Référence: Datasheet ATmega328P
 * - Section 21.5.2 "Bit Rate Generator Unit" p.180
 * - Section 21.9.1 "TWBR – TWI Bit Rate Register" p.198
 */
void   i2c_init(void)
{
    TWBR = 72;
    
    /* TWSR: Prescaler bits (TWPS1:0) = 00 → Prescaler = 1
     * section 21.9.3 "TWSR – TWI Status Register" p.200
     */
    TWSR = 0x00;

    /* Activatio du module TWI 
     * section 21.9.2 TWCR – TWI Control Register p.199
     * Bit 2 – TWEN: TWI Enable Bit 
     */

     TWCR = (1 << TWEN);
}

/*
 * Génère une condition START sur le bus I2C
 * 
 * Une condition START indique le début d'une transmission.
 * Le maître (MCU) prend le contrôle du bus.
 * 
 * Référence: Datasheet ATmega328P
 * - Section 21.6 - p.181 Using the TWI : "1... Writing a one to TWINT clears the flag"
 * - Section 21.9.2 "TWCR – TWI Control Register" :
 *          Bit 7 – TWINT: TWI Interrupt Flag :(écrire 1 pour clear et démarrer l'action)
 *          Bit 5 – TWSTA: TWI START Condition Bit :(génère START)
 *          Bit 2 – TWEN: TWI Enable Bit
 * - Figure 21-10 - p.181 "Interfacing the Application to the TWI in a Typical Transmission"
 * - Table 21-2. Assembly code Example p.183 - ligne 1
 */
void    i2c_start(void)
{
    /* On génère la condition START */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    /* Attendre que TWINT soit mis à 1 (opération terminée)
     * TWINT est clear automatiquement quand l'opération commence
     * et set automatiquement quand elle se termine
     */
    while (!(TWCR & (1 << TWINT)))
        ;
}


/*
 * Génère une condition STOP sur le bus I2C
 * 
 * Une condition STOP indique la fin d'une transmission.
 * Le bus est libéré et d'autres maîtres peuvent le prendre.
 * 
 * Note: TWINT n'est PAS set après STOP (contrairement aux autres opérations)
 * donc on ne fait pas de while() ici.
 * 
 * Référence: Datasheet ATmega328P
 * - Section 21.6 - p.181 Using the TWI : "1... Writing a one to TWINT clears the flag"
 * - Section 21.9.2 "TWCR – TWI Control Register" :
 *          Bit 7 – TWINT: TWI Interrupt Flag :(écrire 1 pour clear et démarrer l'action)
 *          Bit 4 – TWSTO: TWI STOP Condition Bit :(génère STOP)
 *          Bit 2 – TWEN: TWI Enable Bit
 * - Table 21-2. Assembly code Example p.183 - ligne 7
 */
void    i2c_stop(void)
{
        /* On génère la condition STOP */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

/*
 * Envoie un octet sur le bus I2C
 * 
 * On charge la donnée dans TWDR et démarre la transmission.
 * Utilisé pour envoyer l'adresse du slave ou des données.
 * 
 * Référence: Datasheet ATmega328P
 * - Section 21.7.1 "Master Transmitter Mode" p.184
 * - Section 21.9.4 "TWDR – TWI Data Register" p.200
 * - Table 21-2. Assembly code Example p.183 - ligne 5 & 6
 */
void    i2c_write(uint8_t data)
{
    /* on charge la data dans le registre */
    TWDR = data;

    /* On demarre la transmission ( sans START, on envoie juste la data) */
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    /* Attendre la fin de la transmission */
    while (!(TWCR & (1 << TWINT)))
        ;
}


/*
 * Lit un octet et envoie ACK
 * 
 * Utilisé pour lire des octets intermédiaires (pas le dernier).
 * TWEA (TWI Enable Acknowledge) est mis à 1 pour envoyer ACK.
 * 
 * Référence: Datasheet ATmega328P
 * - Section 21.7.2 "Master Receiver Mode" p.188
 * - Table 21-4 "Status Codes for Master Receiver Mode"
 */
uint8_t    i2c_read_ack(void)
{
    /* Activer la réception avec ACK 
     * TWEA (bit 6): TWI Enable Acknowledge
     */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    
    /* Attendre la réception */
    while (!(TWCR & (1 << TWINT)));
    
    /* On retourne la data reçue */
    return TWDR;
}


/*
 * Lit un octet et envoie NACK
 * 
 * Utilisé pour lire le DERNIER octet d'une transmission.
 * TWEA n'est pas mis à 1, donc NACK est envoyé automatiquement.
 * 
 * Référence: Datasheet ATmega328P
 * - Section 21.7.2 "Master Receiver Mode"
 * - Table 21-4 "Status Codes for Master Receiver Mode"
 */
uint8_t     i2c_read_nack(void)
{
    /* Activer la réception SANS ACK (pas de TWEA) */
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    /* Attendre la réception */
    while (!(TWCR & (1 << TWINT)));
    
    /* On retourne la data reçue */
    return TWDR;
}

/*
 * Retourne le code de status I2C
 * 
 * TWSR contient le status sur les 5 bits de poids fort.
 * Les 3 bits de poids faible sont le prescaler (TWPS1:0).
 * On masque avec 0xF8 pour ne garder que le status.
 * 
 * Codes status importants:
 * - 0x08: START transmis
 * - 0x10: Repeated START transmis
 * - 0x18: SLA+W transmis, ACK reçu
 * - 0x20: SLA+W transmis, NACK reçu
 * - 0x28: Data transmis, ACK reçu
 * - 0x30: Data transmis, NACK reçu
 * - 0x40: SLA+R transmis, ACK reçu
 * - 0x48: SLA+R transmis, NACK reçu
 * - 0x50: Data reçu, ACK retourné
 * - 0x58: Data reçu, NACK retourné
 * 
 * Référence: Datasheet ATmega328P
 * - Section 21.9.3 "TWSR – TWI Status Register"
 * - Tables 21-2, 21-3, 21-4 "Status Codes"
 */
uint8_t i2c_get_status(void)
{
    /* Masquer les bits de prescaler (bits 0-2)
     * Ne garder que les bits de status (bits 3-7)
     */
    return (TWSR & 0xF8);
}
