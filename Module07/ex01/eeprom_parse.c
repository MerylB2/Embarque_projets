/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 13:26:24 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 15:30:22 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// Affichage hexdump avec mise en évidence d'une adresse
void eeprom_hexdump_redlight(uint16_t highlight_addr)
{
    uint16_t addr;
    uint8_t byte;
    uint8_t found = 0;
    
    // Parcourir toute l'EEPROM par lignes de 16 octets
    for (addr = 0; addr < EEPROM_SIZE; addr += 16) 
    {
        // Afficher l'adresse
        uart_tx('0');
        uart_tx('0');
        uart_tx('0');
        uart_tx('0');
        uart_printhex_lower((uint8_t)(addr >> 8));
        uart_printhex_lower((uint8_t)(addr & 0xFF));
        uart_tx(' ');
        
        // Afficher 16 octets en MAJUSCULE
        // Lecture EEPROM avec eeprom_read_byte() (avr/eeprom.h autorisé)
        for (uint8_t i = 0; i < 16; i++)
        {
            uint16_t current_addr = addr + i;
            byte = eeprom_read_byte((uint8_t*)current_addr);
            
            // Si c'est l'adresse à mettre en évidence, utiliser la couleur rouge
            if (current_addr == highlight_addr)
            {
                uart_printstr(RED);
                uart_printhex_lower(byte);
                uart_printstr(RESET);
                found = 1;
            } else
                uart_printhex_lower(byte);
            
            // Ajouter un espace après chaque groupe de 2 octets
            if (i % 2 == 1) {
                uart_tx(' ');
            }
        }
        
        uart_tx('\r');
        uart_tx('\n');
        
        if (found) {
            uart_println("...");
            return;
        }
    }
}

// Parse une adresse hexa (ex: "10" -> 0x10)
uint16_t parse_hex_address(const char *str)
{
    uint16_t result = 0;
    
    while (*str)
    {
        result <<= 4;
        if (*str >= '0' && *str <= '9')
            result |= (*str - '0');
        else if (*str >= 'a' && *str <= 'f')
            result |= (*str - 'a' + 10);
        else if (*str >= 'A' && *str <= 'F')
            result |= (*str - 'A' + 10);
        str++;
    }
    
    return result;
}

// Parse un octet hexadécimal (ex: "7f" -> 0x7F)
uint8_t parse_hex_byte(const char *str)
{
    uint8_t result = 0;
    
    for (int i = 0; i < 2 && str[i]; i++)
    {
        result <<= 4;
        if (str[i] >= '0' && str[i] <= '9')
            result |= (str[i] - '0');
        else if (str[i] >= 'a' && str[i] <= 'f')
            result |= (str[i] - 'a' + 10);
        else if (str[i] >= 'A' && str[i] <= 'F')
            result |= (str[i] - 'A' + 10);
    }
    
    return result;
}


