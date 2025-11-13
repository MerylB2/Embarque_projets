/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:12:32 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 16:44:20 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
 *  Affiche le status I2C après chaque envoi de donnée
 * 
 * Cette fonction affiche les valeurs de statut après chaque envoi de donnée,
 * comme demandé dans l'exercice.
 * 
 * Codes importants (Datasheet ATmega328P Tables 21-2, 21-3, 21-4):
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
 */

void print_status(const char *msg)
{
    uint8_t status = TWSR & 0xF8;
    
    uart_printstr(msg);
    uart_printstr(" - Status: ");
    uart_printhex(status);
    
    // Interprétation du status
    switch (status)
    {
        case 0x08:
            uart_println(" - START condition transmitted");
            break;
        case 0x10:
            uart_println(" - Repeated START condition transmitted");
            break;
        case 0x18:
            uart_println(" - SLA+W transmitted, ACK received");
            break;
        case 0x20:
            uart_println(" - SLA+W transmitted, NACK received");
            break;
        case 0x28:
            uart_println(" - Data byte transmitted, ACK received");
            break;
        case 0x30:
            uart_println(" - Data byte transmitted, NACK received");
            break;
        case 0x38:
            uart_println(" - Arbitration lost in SLA+W or data");
            break;
        case 0x40:
            uart_println(" - SLA+R transmitted, ACK received");
            break;
        case 0x48:
            uart_println(" - SLA+R transmitted, NACK received");
            break;
        case 0x50:
            uart_println(" - Data byte received, ACK returned");
            break;
        case 0x58:
            uart_println(" - Data byte received, NACK returned");
            break;
        default:
            uart_println(" - Unexpected status");
            break;
    }
}

/*
 * Fonction helper pour envoyer un octet via I2C
 */
void send_byte(uint8_t data)
{
    // Écrire dans TWDR
    TWDR = data;
    
    // Démarrer la transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    // Attendre la fin
    while (!(TWCR & (1 << TWINT)));
}

int main(void)
{
    uart_init();
    i2c_init();
    
    uart_println("\n==========================================");
    uart_println("  EX00 - Communication I2C/AHT20");
    uart_println("  Affichage des status apres chaque envoi");
    uart_println("==========================================\n");
    
    /* Séquence d'initialisation du capteur AHT20
     * Datasheet AHT20 section 5.4 step 1):
     */
    uart_println("=== Initialisation AHT20 ===");
    
    // START
    i2c_start();
    print_status("START");
    
    // Adresse + Write
    send_byte((AHT20_ADDR << 1) | 0);
    print_status("Adresse+W");
    
    // Commande init: 0xBE 0x08 0x00
    send_byte(0xBE);
    print_status("CMD 0xBE");
    
    send_byte(0x08);
    print_status("Param 0x08");
    
    send_byte(0x00);
    print_status("Param 0x00");
    
    // STOP
    i2c_stop();
    uart_println("STOP\n");
    
    /* Séquence trigger mesure 
     * Datasheet AHT20 section 5.4 step 2
     */ 
    uart_println("=== Trigger mesure ===");
    
    i2c_start();
    print_status("START");
    
    send_byte((AHT20_ADDR << 1) | 0);
    print_status("Adresse+W");
    
    // Commande trigger: 0xAC 0x33 0x00
    send_byte(0xAC);
    print_status("CMD 0xAC");
    
    send_byte(0x33);
    print_status("Param 0x33");
    
    send_byte(0x00);
    print_status("Param 0x00");
    
    i2c_stop();
    uart_println("STOP\n");
    
    uart_println("==========================================");
    uart_println("  Communication terminee");
    uart_println("==========================================\n");
    
    while (1)
    {}
    
    return 0;
}
