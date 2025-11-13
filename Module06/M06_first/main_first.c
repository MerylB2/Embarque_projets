/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:31:05 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/12 17:34:51 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
 * print_status - Affiche le code de status I2C
 * 
 * Codes importants (Datasheet ATmega328P Tables 21-2, 21-3, 21-4):
 * - 0x08: START transmis
 * - 0x18: SLA+W transmis, ACK reçu
 * - 0x20: SLA+W transmis, NACK reçu (device absent)
 * - 0x28: Data transmis, ACK reçu
 * - 0x40: SLA+R transmis, ACK reçu
 */
void print_status(const char *msg)
{
    uint8_t status = i2c_get_status();
    
    uart_printstr(msg);
    uart_printstr(" Status: ");
    uart_printhex(status);
    
    // Interprétation du status
    switch (status)
    {
        case 0x08:
            uart_println(" [START OK]");
            break;
        case 0x10:
            uart_println(" [REPEATED START OK]");
            break;
        case 0x18:
            uart_println(" [SLA+W ACK]");
            break;
        case 0x20:
            uart_println(" [SLA+W NACK - Device not found]");
            break;
        case 0x28:
            uart_println(" [DATA ACK]");
            break;
        case 0x30:
            uart_println(" [DATA NACK]");
            break;
        case 0x38:
            uart_println(" [ARBITRATION LOST]");
            break;
        case 0x40:
            uart_println(" [SLA+R ACK]");
            break;
        case 0x48:
            uart_println(" [SLA+R NACK]");
            break;
        default:
            uart_println(" [UNKNOWN]");
            break;
    }
}

/*
 * aht20_init - Initialise le capteur AHT20
 * 
 * Séquence (Datasheet AHT20):
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
 * Séquence (Datasheet AHT20):
 * 1. Envoyer commande 0xAC 0x33 0x00
 * 2. Attendre 75ms minimum pour la mesure
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
    
    // Attendre la mesure (75ms minimum selon datasheet)
    _delay_ms(80);
}

/*
 * aht20_read_data - Lit les données du capteur (7 octets)
 * 
 * Format (Datasheet AHT20):
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

int main(void)
{
    // Initialisation
    uart_init();
    i2c_init();
    
    uart_println("\n\n==========================================");
    uart_println("  EX00 - I2C Communication Test");
    uart_println("  Device: AHT20 Temperature & Humidity");
    uart_println("  I2C Frequency: 100kHz");
    uart_println("==========================================\n");
    
    // Séquence de test
    aht20_init();
    
    _delay_ms(500);
    
    aht20_trigger_measurement();
    
    _delay_ms(500);
    
    aht20_read_data();
    
    uart_println("==========================================");
    uart_println("  Test Complete!");
    uart_println("==========================================\n");
    
    // Boucle infinie
    while (1) {
        _delay_ms(1000);
    }
    
    return 0;
}