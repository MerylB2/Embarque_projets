/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:12:32 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/12 18:27:01 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
 * print_float - Affiche un nombre flottant avec 2 décimales
 */
void print_float(float value)
{
    int16_t integer = (int16_t)value;
    uint16_t decimal = (uint16_t)((value - integer) * 100);
    
    // Gérer les nombres négatifs
    if (value < 0 && integer == 0)
    {
        uart_tx('-');
    }
    
    // Partie entière
    if (integer < 0)
    {
        uart_tx('-');
        integer = -integer;
    }
    
    // Convertir l'entier en chaîne
    char buffer[6];
    int i = 0;
    if (integer == 0)
        buffer[i++] = '0';
    else 
    {
        int temp = integer;
        int digits = 0;
        while (temp > 0) {
            buffer[digits++] = '0' + (temp % 10);
            temp /= 10;
        }
        // Inverser
        for (int j = digits - 1; j >= 0; j--)
        {
            uart_tx(buffer[j]);
        }
    }
    
    // Point décimal
    uart_tx('.');
    
    // Partie décimale (2 chiffres)
    uart_tx('0' + (decimal / 10));
    uart_tx('0' + (decimal % 10));
}

int main(void) {
    aht20_data_t sensor_data;
    
    // Initialisation
    uart_init();
    i2c_init();
    
    uart_println("\n\n==========================================");
    uart_println("  EX00 - I2C Communication Test");
    uart_println("  Device: AHT20 Temperature & Humidity");
    uart_println("  I2C Frequency: 100kHz");
    uart_println("==========================================\n");
    
    // Initialiser le capteur AHT20
    uart_println("=== AHT20 Initialization ===");
    aht20_init();
    uart_println("Initialization complete!\n");
    
    _delay_ms(500);
    
    // Boucle de lecture continue
    uart_println("=== Starting continuous readings ===\n");
    
    while (1)
    {
        // Lire température et humidité
        aht20_read(&sensor_data);
        
        // Vérifier si le capteur était busy
        if (aht20_is_busy(sensor_data.status))
            uart_println("Warning: Sensor was still busy!");
        
        // Afficher les résultats
        uart_printstr("Temperature: ");
        print_float(sensor_data.temperature);
        uart_printstr(" C | Humidity: ");
        print_float(sensor_data.humidity);
        uart_printstr(" % | Status: ");
        uart_printhex(sensor_data.status);
        
        // Afficher les bits de status
        uart_printstr(" [");
        if (aht20_is_busy(sensor_data.status))
            uart_printstr("BUSY ");

        if (aht20_is_calibrated(sensor_data.status))
            uart_printstr("CAL");

        uart_println("]");
        
        // Attendre 2 secondes avant la prochaine lecture
        _delay_ms(2000);
    }
    
    return 0;
}