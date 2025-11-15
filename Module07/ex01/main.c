/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:02:28 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 14:49:17 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int main(void)
{
    char buffer[32];
    char addr_str[16];
    char value_str[16];
    uint8_t buf_idx;
    uint8_t addr_idx, value_idx;
    char c;
    uint16_t addr;
    uint8_t value;
    
    uart_init();
    
    // Délai pour stabilisation
    for (volatile uint32_t i = 0; i < 100000; i++);
    
    while (1) {
        // Lecture de la ligne complète (adresse + valeur)
        buf_idx = 0;
        while (1) {
            c = uart_rx();
            
            // Backspace (0x7F ou 0x08)
            if (c == 0x7F || c == 0x08) {
                if (buf_idx > 0) {
                    buf_idx--;
                    uart_tx('\b');
                    uart_tx(' ');
                    uart_tx('\b');
                }
                continue;
            }
            
            // Enter
            if (c == '\r' || c == '\n') {
                uart_tx('\r');
                uart_tx('\n');
                buffer[buf_idx] = '\0';
                break;
            }
            
            // Caractère normal
            if (buf_idx < sizeof(buffer) - 1) {
                buffer[buf_idx++] = c;
                uart_tx(c); // Echo
            }
        }
        
        // Parser la ligne pour extraire adresse et valeur
        // Format attendu: "adresse valeur" (ex: "10 7f")
        addr_idx = 0;
        value_idx = 0;
        uint8_t i = 0;
        
        // Lire l'adresse (jusqu'à l'espace)
        while (buffer[i] && buffer[i] != ' ' && addr_idx < sizeof(addr_str) - 1) {
            addr_str[addr_idx++] = buffer[i++];
        }
        addr_str[addr_idx] = '\0';
        
        // Sauter les espaces
        while (buffer[i] == ' ') {
            i++;
        }
        
        // Lire la valeur
        while (buffer[i] && value_idx < sizeof(value_str) - 1) {
            value_str[value_idx++] = buffer[i++];
        }
        value_str[value_idx] = '\0';
        
        // Vérifier qu'on a bien les deux parties
        if (addr_idx == 0 || value_idx == 0) {
            continue;
        }
        
        addr = parse_hex_address(addr_str);
        
        // Vérifier que l'adresse est valide
        if (addr >= EEPROM_SIZE) {
            uart_println("Invalid address!");
            continue;
        }
        
        value = parse_hex_byte(value_str);
        
        // Vérifier si la valeur existe déjà
        // Si oui, ne rien faire (comme indiqué dans l'énoncé)
        uint8_t current_value = eeprom_read_byte((uint8_t*)addr);
        
        if (current_value != value) {
            // Écrire dans l'EEPROM - eeprom_write_byte() (avr/eeprom.h autorisé)
            // Info: nombre de cycles d'écriture limité (~100,000 cycles)
            eeprom_write_byte((uint8_t*)addr, value);
        }
        
        // Afficher le hexdump avec la valeur en rouge
        eeprom_hexdump_redlight(addr);
    }
    
    return 0;
}

/*
 * Interprétation de la sortie
 * Le format word affiche les octets par groupes de 2 sans espace entre eux. 
 * Exple : Quand vous écrivez 7f à l'adresse 0x24 :
 * 	La ligne commence à 0x20 (alignée sur 16 octets)
 * 	Les adresses s'affichent par paires : 0x20+0x21 → 00ff, 0x22+0x23 → ffff, 0x24+0x25 → 7fff
 * 	Le 7f (en rouge) apparaît à gauche du groupe 7fff car il est à l'adresse paire 0x24
 * 	L'octet suivant 0x25 contient ff (valeur par défaut de l'EEPROM vierge)
 */