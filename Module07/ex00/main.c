/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:02:28 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 14:05:19 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// Affichage hexdump de l'EEPROM
void eeprom_hexdump(void)
{
    uint16_t addr;
    uint8_t byte;
    
    // Parcourir toute l'EEPROM par lignes de 16 octets
    for (addr = 0; addr < EEPROM_SIZE; addr += 16)
    {
        // Afficher l'adresse (7 chiffres comme dans l'exemple: 0000000)
        uart_tx('0');
        uart_tx('0');
        uart_tx('0');
        uart_tx('0');
        uart_printhex((uint8_t)(addr >> 8));
        uart_printhex((uint8_t)(addr & 0xFF));
        uart_tx(' ');
        
        // Afficher 16 octets
        // eeprom_read_byte() de avr/eeprom.h (autorisé)
        // Lecture EEPROM - Datasheet p.19-21 (7.4.1 EEPROM Read/Write Access)
        for (uint8_t i = 0; i < 16; i++)
        {
            byte = eeprom_read_byte((uint8_t*)(addr + i));
            uart_printhex_lower(byte);
            // Ajouter un espace après chaque groupe de 2 octets
            if (i % 2 == 1)
                uart_tx(' ');
        }
        
        uart_tx('\r');
        uart_tx('\n');
    }
    // Fin du dump
}

int main(void)
{
    uart_init();
    
    // Petit délai pour stabilisation
    for (volatile uint32_t i = 0; i < 100000; i++);
    
    // Affichage du contenu de l'EEPROM
    eeprom_hexdump();
    
    while (1) {};
    
    return 0;
}


/* D'après le schéma fourni :
 *
 * UART : RX sur PD0, TX sur PD1 (connecté au CH340G USB-serial)
 * F_CPU : 16MHz (quartz Y1)
 * EEPROM : 1024 octets intégrés dans l'ATmega328P
 */

/* Analyse de la Sortie 
 *
 * EEPROM vierge = 0xFF ou 0xff partout
 *
 * Une EEPROM jamais écrite ou effacée contient 0xFF (tous les bits à 1) car flash
 * C'est l'état naturel de la mémoire flash/EEPROM
 * Parcours complet de 0x000 à 0x3FF (000003F0 ffff ffff ffff ffff ffff ffff ffff ffff 
 * = 1008 + 16 = 1024 octets)
 * 
 */