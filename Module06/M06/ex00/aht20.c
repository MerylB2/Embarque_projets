/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aht20.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:36:07 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/12 19:08:58 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf

/*
 * aht20_init - Initialise le capteur AHT20
 * 
 * Séquence (Datasheet AHT20 section 5.4 step 1):
 * 1. Attendre 40ms après power-on
 * 2. Envoyer commande 0xBE 0x08 0x00
 * 3. Attendre 10ms
 */
void aht20_init(void)
{
    uart_println("\n=== AHT20 Initialization ===");
    
    // Attendre 40ms après power-on (datasheet requirement)
    _delay_ms(40);
    
    // START condition
    i2c_start();
    print_status("START");
    
    // Envoyer adresse AHT20 en mode WRITE
    i2c_write((AHT20_ADDR << 1) | 0);
    print_status("Address Write");
    
    // Envoyer commande d'initialisation: 0xBE 0x08 0x00
    i2c_write(AHT20_INIT_CMD);
    print_status("Init CMD (0xBE)");
    
    i2c_write(0x08);
    print_status("Param 1 (0x08)");
    
    i2c_write(0x00);
    print_status("Param 2 (0x00)");
    
    // STOP condition
    i2c_stop();
    uart_println("STOP\n");
    
    // Attendre 10ms (datasheet requirement)
    _delay_ms(10);
}

/*
 * aht20_trigger_measurement - Déclenche une mesure
 * 
 * Séquence (Datasheet AHT20 section 5.4 step 2):
 * 1. Envoyer commande 0xAC 0x33 0x00
 * 2. Attendre 80ms minimum pour la mesure
 */
void aht20_trigger_measurement(void)
{
    uart_println("=== Trigger Measurement ===");
    
    // START condition
    i2c_start();
    print_status("START");
    
    // Adresse en mode WRITE
    i2c_write((AHT20_ADDR << 1) | 0);
    print_status("Address Write");
    
    // Commande trigger: 0xAC 0x33 0x00
    i2c_write(AHT20_TRIGGER_CMD);
    print_status("Trigger CMD (0xAC)");
    
    i2c_write(0x33);
    print_status("Param 1 (0x33)");
    
    i2c_write(0x00);
    print_status("Param 2 (0x00)");
    
    // STOP condition
    i2c_stop();
    uart_println("STOP\n");
    
    // Attendre la mesure (80ms minimum selon datasheet)
    _delay_ms(80);
}

/*
 * aht20_read_data - Lit les données du capteur (7 octets)
 * 
 * Format (Datasheet AHT20 section 5.4 step 4):
 * Byte 0: Status (bit 7 = busy)
 * Byte 1-3: Humidité (20 bits)
 * Byte 3-5: Température (20 bits)
 * Byte 6: CRC
 */
void aht20_read_data(void)
{
    uint8_t data[7];
    
    uart_println("=== Read Data (7 bytes) ===");
    
    // START condition
    i2c_start();
    print_status("START");
    
    // Adresse en mode READ
    i2c_write((AHT20_ADDR << 1) | 1);
    print_status("Address Read");
    
    // Lire 6 premiers octets avec ACK
    for (uint8_t i = 0; i < 6; i++) {
        data[i] = i2c_read_ack();
        uart_printstr("Data[");
        uart_tx('0' + i);
        uart_printstr("]: ");
        uart_printhex(data[i]);
        uart_println("");
    }
    
    // Lire le 7ème octet avec NACK (dernier)
    data[6] = i2c_read_nack();
    uart_printstr("Data[6]: ");
    uart_printhex(data[6]);
    uart_println(" [NACK sent]");
    
    // STOP condition
    i2c_stop();
    uart_println("STOP\n");
    
    // Vérifier si busy
    if (data[0] & 0x80) {
        uart_println("Warning: Sensor still busy!");
    }
}
