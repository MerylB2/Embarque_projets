/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:30:41 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 14:35:26 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


void print_status(const char *msg)
{
    uint8_t status = i2c_get_status();
    
    uart_printstr(msg);
    uart_printstr(" Status: ");
    uart_printhex(status);
    
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
        case 0x28:
            uart_println(" [DATA ACK]");
            break;
        case 0x40:
            uart_println(" [SLA+R ACK]");
            break;
        default:
            uart_println("");
            break;
    }
}
/*
 * Test 3: REPEATED START (0x10)
 * 
 * Pour obtenir un REPEATED START, il faut:
 * 1. Faire un START
 * 2. Envoyer une adresse
 * 3. Faire un autre START SANS faire de STOP entre les deux
 * 
 * C'est utilisé pour changer de direction (Write → Read) sans libérer le bus
 */
void test_repeated_start(void)
{
    uart_println("==========================================");
    uart_println("TEST 3: REPEATED START");
    uart_println("==========================================");
    
    // Premier START
    i2c_start();
    print_status("START");
    
    // Adresse en mode Write
    i2c_write((AHT20_ADDR << 1) | 0);
    print_status("SLA+W");
    
    // REPEATED START (sans STOP!)
    i2c_start();
    print_status("REPEATED START");  // Devrait afficher 0x10
    
    // On peut maintenant changer de mode (ex: Read)
    i2c_write((AHT20_ADDR << 1) | 1);
    print_status("SLA+R");
    
    // STOP final
    i2c_stop();
    uart_println("STOP\n");
}


/*
 * Test 2: ARBITRATION LOST (0x38)
 * 
 * L'arbitration lost se produit quand:
 * - Plusieurs maîtres essaient de communiquer en même temps
 * - Notre ATmega328P perd l'arbitrage
 * 
 * IMPOSSIBLE à tester avec un seul maître!
 * Il faudrait deux microcontrôleurs sur le même bus I2C.
 */
void test_arbitration_lost(void)
{
    uart_println("\n==========================================");
    uart_println("TEST 2: ARBITRATION LOST");
    uart_println("==========================================");
    uart_println("IMPOSSIBLE a tester avec un seul maitre!");
    uart_println("Il faudrait 2 MCU sur le meme bus I2C.\n");
    uart_println("Scenario:");
    uart_println("- MCU1 et MCU2 envoient START simultanement");
    uart_println("- Les deux ecrivent des adresses differentes");
    uart_println("- Celui qui ecrit un '1' alors que l'autre ecrit '0'");
    uart_println("  detecte une collision et perd l'arbitrage");
    uart_println("- Status 0x38 [ARBITRATION LOST] est genere\n");
}

/*
 * Test 1: UNKNOWN status
 * 
 * Pour obtenir un status inconnu:
 * - Lire le status à un moment non approprié
 * - Ou provoquer une erreur dans la séquence I2C
 */
void test_unknown_status(void)
{
    uart_println("\n==========================================");
    uart_println("TEST 1: UNKNOWN STATUS");
    uart_println("==========================================");
    uart_println("Status avant toute operation I2C:\n");
    
    // Lire le status sans avoir initialisé
    uint8_t status = TWSR & 0xF8;
    uart_printstr("Status: ");
    uart_printhex(status);
    
    if (status == 0xF8)
        uart_println(" [No relevant state - UNKNOWN]");
    else
        uart_println("");
    uart_println("");
}

int main(void)
{
    // Initialisation
    uart_init();
    i2c_init();
    
    uart_println("\n\n==========================================");
    uart_println("    Autres cas de Test sur l' I2C/TWI");
    uart_println("==========================================\n");
    
    _delay_ms(1000);
    
    // Test UNKNOWN avant init
    test_unknown_status();
    _delay_ms(1000);

    // test Arbritration lost
    test_arbitration_lost();
    _delay_ms(1000);
    
    // Init I2C
    uart_println("Initialisation I2C...");
    i2c_init();
    uart_println("OK\n");
    _delay_ms(1000);
    
    // Test REPEATED START
    test_repeated_start();
    _delay_ms(1000);
    
    uart_println("\n==========================================");
    uart_println("  Tests termines!");
    uart_println("==========================================\n");
    
    while (1)
    {
        _delay_ms(1000);
    }
    
    return 0;
}
