/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:12:32 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 17:07:46 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int main(void)
{
    uint8_t sensor_initialized = 0;
    uart_init();
    i2c_init();
    
    // Attendre 40ms après power-on
    // Référence: AHT20 Datasheet section 5.4 "Sensor Reading Process"
    _delay_ms(40);
    
    while (1)
    {
        // Vérifier si le capteur est initialisé
        if (!sensor_initialized)
        {
            // Lire le status byte pour vérifier la calibration
            i2c_start();
            i2c_write((AHT20_ADDR << 1) | I2C_READ);
            i2c_read();
            uint8_t status = TWDR;
            i2c_stop();

             // Bit 3 = CAL (calibrated)
            if (!(status & (1 << 3)))
                aht20_init();
            else
                sensor_initialized = 1;
        }
        // Buffer pour stocker les 7 octets
        char data[7];
        
        //trigger mesure
        aht20_trigger_measurement();
        
        // Lecture des 7 octets
        // Référence: AHT20 Datasheet section 5.4 step 4
        i2c_start();
        i2c_write((AHT20_ADDR << 1) | I2C_READ);
        
        // Lire les 6 premiers octets avec ACK
        for (uint8_t i = 0; i < 6; i++)
        {
            i2c_read();  // Lit dans TWDR avec ACK
            data[i] = TWDR;
        }
        
        // Lire le 7ème octet avec NACK (dernier)
        i2c_read_last();
        data[6] = TWDR;
        
        i2c_stop();
        
        // Afficher les 7 octets avec print_hex_value
        print_hex_value(data);
        
        // Attendre 1 seconde avant la prochaine mesure
        // (fréquence recommandée pour éviter de saturer le capteur)
        _delay_ms(1000);
    }
    
    return 0;
}
