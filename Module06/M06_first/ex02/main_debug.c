/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_debug.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:09:33 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/12 18:33:53 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


/*
 * print_status - Affiche le code de status I2C après chaque opération
 * 
 * Cette fonction est utile pour le debugging et la compréhension du protocole I2C.
 * Elle affiche le code de status hexadécimal et son interprétation.
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
    
    // Interprétation du status selon les tables du datasheet ATmega328P
    switch (status) {
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
        case 0x50:
            uart_println(" [DATA RECEIVED ACK]");
            break;
        case 0x58:
            uart_println(" [DATA RECEIVED NACK]");
            break;
        default:
            uart_println(" [UNKNOWN]");
            break;
    }
}

/*
 * test_aht20_with_status - Test complet du AHT20 avec affichage des status
 * 
 * Cette fonction reproduit manuellement les étapes de communication avec l'AHT20
 * en affichant le status I2C après chaque opération.
 * 
 * Ceci correspond exactement à ce qui est demandé dans l'exercice:
 * "Le programme devra retourner sur votre ordinateur les valeurs de statut 
 *  après chaque envoi de donnée."
 */
void test_aht20_with_status(void)
{
    uint8_t data[7];
    
    uart_println("\n=== AHT20 Initialization Sequence ===");
    uart_println("Waiting 40ms after power-on...");
    _delay_ms(40);
    
    // Vérification de la calibration (optionnel au démarrage)
    uart_println("\nChecking calibration status:");
    i2c_start();
    print_status("START");
    
    i2c_write((AHT20_ADDR << 1) | 1);
    print_status("Address+Read");
    
    uint8_t status = i2c_read_nack();
    i2c_stop();
    uart_printstr("Status byte: ");
    uart_printhex(status);
    uart_println("");
    
    if (!(status & AHT20_STATUS_CALIBRATED))
    {
        uart_println("\nSensor not calibrated, sending init command...");
        i2c_start();
        print_status("START");
        
        i2c_write((AHT20_ADDR << 1) | 0);
        print_status("Address+Write");
        
        i2c_write(AHT20_CMD_INIT);
        print_status("Init CMD (0xBE)");
        
        i2c_write(0x08);
        print_status("Param 1 (0x08)");
        
        i2c_write(0x00);
        print_status("Param 2 (0x00)");
        
        i2c_stop();
        uart_println("STOP\n");
        
        _delay_ms(10);
    } else 
    {
        uart_println("Sensor already calibrated!\n");
    }
    
    _delay_ms(500);
    
    // Déclenchement de la mesure
    uart_println("=== Trigger Measurement ===");
    i2c_start();
    print_status("START");
    
    i2c_write((AHT20_ADDR << 1) | 0);
    print_status("Address+Write");
    
    i2c_write(AHT20_CMD_TRIGGER);
    print_status("Trigger CMD (0xAC)");
    
    i2c_write(0x33);
    print_status("Param 1 (0x33)");
    
    i2c_write(0x00);
    print_status("Param 2 (0x00)");
    
    i2c_stop();
    uart_println("STOP\n");
    
    uart_println("Waiting 80ms for measurement...");
    _delay_ms(80);
    
    // Lecture des données
    uart_println("=== Read Data (7 bytes) ===");
    i2c_start();
    print_status("START");
    
    i2c_write((AHT20_ADDR << 1) | 1);
    print_status("Address+Read");
    
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
    
    i2c_stop();
    uart_println("STOP\n");
    
    // Conversion des données
    uart_println("=== Data Conversion ===");
    
    aht20_data_t result;
    aht20_convert_data(data, &result);
    
    uart_printstr("Status byte: ");
    uart_printhex(result.status);
    uart_printstr(" [");
    if (aht20_is_busy(result.status)) uart_printstr("BUSY ");
    if (aht20_is_calibrated(result.status)) uart_printstr("CAL");
    uart_println("]");
    
    // Affichage des valeurs brutes
    uint32_t hum_raw = ((uint32_t)data[1] << 12) | 
                       ((uint32_t)data[2] << 4) | 
                       (data[3] >> 4);
    uint32_t temp_raw = (((uint32_t)data[3] & 0x0F) << 16) | 
                        ((uint32_t)data[4] << 8) | 
                        data[5];
    
    uart_printstr("Humidity raw (20 bits): ");
    uart_printhex(hum_raw >> 16);
    uart_printhex(hum_raw >> 8);
    uart_printhex(hum_raw);
    uart_println("");
    
    uart_printstr("Temperature raw (20 bits): ");
    uart_printhex(temp_raw >> 16);
    uart_printhex(temp_raw >> 8);
    uart_printhex(temp_raw);
    uart_println("\n");
}

int main(void)
{
    // Initialisation
    uart_init();
    i2c_init();
    
    uart_println("==========================================");
    uart_println("  EX00 - I2C Communication Test");
    uart_println("  Device: AHT20 Temperature & Humidity");
    uart_println("  I2C Frequency: 100kHz");
    uart_println("  MODE: Detailed Status Display");
    uart_println("==========================================");
    
    // Test avec affichage détaillé des status
    test_aht20_with_status();
    
    uart_println("\n==========================================");
    uart_println("  Test Complete!");
    uart_println("==========================================\n");
    
    // Boucle infinie
    while (1) {
        _delay_ms(1000);
    }
    
    return 0;
}