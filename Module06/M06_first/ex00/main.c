/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:12:32 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/12 20:20:59 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
 * Affiche le code de status I2C après chaque opération
 * 
 * Cette fonction affiche les valeurs de statut après chaque envoi de donnée,
 * comme demandé dans l'exercice.
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

int main(void)
{
    uart_init();
    i2c_init();
    
    uart_println("==========================================");
    uart_println("  EX00 - TWI/I2C Communication Test");
    uart_println("  Device: AHT20 Temperature & Humidity");
    uart_println("  I2C Frequency: 100kHz");
    uart_println("==========================================\n");
    
    // Séquence de test complète avec affichage des status
    aht20_init();
    
    _delay_ms(500);
    
    aht20_trigger_measurement();
    
    _delay_ms(500);
    
    aht20_read_data();
    
    uart_println("==========================================");
    uart_println("  Test Complete!");
    uart_println("==========================================\n");
    
    while (1) 
    {
        _delay_ms(1000);
    }
    return 0;
}