/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:06:53 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 17:28:42 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// datasheet : https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf


void   i2c_init(void)
{
    // Configuration des pins SDA (PC4) et SCL (PC5) en INPUT
    // Datasheet section 21.3: "SDA and SCL pins must be configured as inputs"
    DDRC &= ~(1 << PC4);  // SDA en INPUT
    DDRC &= ~(1 << PC5);  // SCL en INPUT
    
    // Pas de pull-up interne (les pull-ups externes de la carte suffisent)
    PORTC &= ~(1 << PC4);
    PORTC &= ~(1 << PC5);
    
    // Configuration du bit rate pour 100kHz
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
 * On Génère une condition START sur le bus I2C
 * 
 * Une condition START indique le début d'une transmission.
 * Le maître (MCU) prend le contrôle du bus.
 * 
 * Réfs :
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


void    i2c_stop(void)
{
        /* On génère la condition STOP */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}


void    i2c_write(unsigned char data)
{
    /* on charge la data dans le registre */
    TWDR = data;

    /* On demarre la transmission */
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    /* Attendre la fin */
    while (!(TWCR & (1 << TWINT)))
        ;
}


void   i2c_read(void)
{
    /* Activer la réception avec ACK 
     * TWEA (bit 6): TWI Enable Acknowledge
     */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    
    /* Attendre la réception */
    while (!(TWCR & (1 << TWINT)))
        ;
    
    // Le résultat est maintenant dans TWDR
    // (L'affichage sera fait par print_hex dans le main)
}


void    i2c_read_last(void)
{
    /* Activer la réception SANS ACK (pas de TWEA) */
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    /* Attendre la réception */
    while (!(TWCR & (1 << TWINT)))
        ;
    
    // Le résultat est dans TWDR
}


uint8_t i2c_get_status(void)
{
    return (TWSR & 0xF8);
}
