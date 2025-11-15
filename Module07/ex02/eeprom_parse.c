/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 13:26:24 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 21:16:24 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// Recherche une clé dans l'EEPROM
uint16_t find_key(const char *key, uint16_t *data_addr)
{
    uint16_t addr = 0;
    uint8_t key_len = ft_strlen(key);
    
    while (addr < EEPROM_SIZE) {
        uint8_t magic = eeprom_read_byte((uint8_t*)addr);
        
        // Fin de l'EEPROM utilisée
        if (magic == 0xFF) {
            break;
        }
        
        // Sauvegarder l'adresse du magic byte
        uint16_t magic_addr = addr;
        addr++;
        
        // Lire la longueur de la clé
        uint8_t stored_key_len = eeprom_read_byte((uint8_t*)addr);
        addr++;
        
        // Paire supprimée, passer à la suivante
        if (magic == 0x00) {
            addr += stored_key_len; // Sauter la clé
            uint8_t val_len = eeprom_read_byte((uint8_t*)addr);
            addr++;
            addr += val_len; // Sauter la valeur
            continue;
        }
        
        // Vérifier si c'est une paire valide
        if (magic != MAGIC_BYTE) {
            break; // Données corrompues
        }
        
        // Comparer les clés
        if (stored_key_len == key_len) {
            uint8_t match = 1;
            for (uint8_t i = 0; i < key_len; i++) {
                if (eeprom_read_byte((uint8_t*)(addr + i)) != key[i]) {
                    match = 0;
                    break;
                }
            }
            
            if (match) {
                // Clé trouvée !
                addr += key_len;
                addr++;
                *data_addr = addr; // Adresse de la valeur
                return magic_addr; // Retourne l'adresse du magic byte
            }
        }
        
        // Pas la bonne clé, passer à la paire suivante
        addr += stored_key_len; // Sauter la clé
        uint8_t val_len = eeprom_read_byte((uint8_t*)addr);
        addr++;
        addr += val_len; // Sauter la valeur
    }
    
    return 0xFFFF; 
}

// Trouve un espace libre
uint16_t find_free_space(void)
{
    uint16_t addr = 0;
    
    while (addr < EEPROM_SIZE)
    {
        uint8_t magic = eeprom_read_byte((uint8_t*)addr);
        
        if (magic == 0xFF)
            return addr;
            
        addr++;
        uint8_t key_len = eeprom_read_byte((uint8_t*)addr++);
        addr += key_len;
        uint8_t val_len = eeprom_read_byte((uint8_t*)addr++);
        addr += val_len;
    }
    
    return 0xFFFF;
}

// READ clé
void cmd_read(const char *key)
{
    uint16_t data_addr;
    uint16_t found = find_key(key, &data_addr);
    
    if (found == 0xFFFF) {
        uart_println("empty");
        return;
    }
    
    uint8_t val_len = eeprom_read_byte((uint8_t*)(data_addr - 1));
    uart_tx('"');
    for (uint8_t i = 0; i < val_len; i++) {
        uart_tx(eeprom_read_byte((uint8_t*)(data_addr + i)));
    }
    uart_tx('"');
    uart_tx('\r');
    uart_tx('\n');
}

// WRITE clé valeur
void cmd_write(const char *key, const char *value)
{
    uint8_t key_len = ft_strlen(key);
    uint8_t val_len = ft_strlen(value);
    
    if (key_len == 0) {
        uart_println("invalid key");
        return;
    }
    
    if (val_len == 0) {
        uart_println("invalid value");
        return;
    }
    
    if (key_len > MAX_STRING_LEN || val_len > MAX_STRING_LEN) {
        uart_println("too long");
        return;
    }
    
    uint16_t data_addr;
    if (find_key(key, &data_addr) != 0xFFFF) {
        uart_println("already exists");
        return;
    }
    
    uint16_t free_addr = find_free_space();
    uint16_t needed = 3 + key_len + val_len;
    
    if (free_addr == 0xFFFF || (free_addr + needed) > EEPROM_SIZE) {
        uart_println("no space left");
        return;
    }
    
    uint16_t addr = free_addr;
    eeprom_write_byte((uint8_t*)addr++, MAGIC_BYTE);
    eeprom_write_byte((uint8_t*)addr++, key_len);
    for (uint8_t i = 0; i < key_len; i++) {
        eeprom_write_byte((uint8_t*)addr++, key[i]);
    }
    eeprom_write_byte((uint8_t*)addr++, val_len);
    for (uint8_t i = 0; i < val_len; i++) {
        eeprom_write_byte((uint8_t*)addr++, value[i]);
    }
    
    uart_println("done");
}

// FORGET clé
void cmd_forget(const char *key)
{
    uint16_t data_addr;
    uint16_t found = find_key(key, &data_addr);
    
    if (found == 0xFFFF) {
        uart_println("not found");
        return;
    }
    
    eeprom_write_byte((uint8_t*)found, 0x00);
    uart_println("done");
}

