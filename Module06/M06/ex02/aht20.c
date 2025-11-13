/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aht20.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:36:07 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 17:55:25 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf


void aht20_init(void)
{    
    // START condition
    i2c_start();
    
    // Envoyer adresse AHT20 en mode WRITE
    i2c_write((AHT20_ADDR << 1) | I2C_WRITE);
    
    // Envoyer commande d'initialisation: 0xBE 0x08 0x00
    i2c_write(AHT20_INIT_CMD);
    i2c_write(0x08);
    i2c_write(0x00);
    
    // STOP condition
    i2c_stop();
    
    // Attendre 10ms (datasheet requirement)
    _delay_ms(10);
}

void aht20_trigger_measurement(void)
{   
    // START condition
    i2c_start();
    
    // Adresse en mode WRITE
    i2c_write((AHT20_ADDR << 1) | I2C_WRITE);
    
    // Commande trigger: 0xAC 0x33 0x00
    i2c_write(AHT20_TRIGGER_CMD);
    i2c_write(0x33);
    i2c_write(0x00);
    
    // STOP condition
    i2c_stop();
    
    // Attendre la mesure (80ms minimum selon datasheet)
   _delay_ms(80);
}

/*
 * Calcule l'humidité relative en % à partir des données brutes
 * 
 * Format des données AHT20 (7 octets):
 * [0]: Status byte
 * [1]: Humidité [19:12]
 * [2]: Humidité [11:4]
 * [3]: Humidité [3:0] (bits 7:4) | Température [19:16] (bits 3:0)
 * [4]: Température [15:8]
 * [5]: Température [7:0]
 * [6]: CRC
 * 
 * Référence: AHT20 Datasheet section "Data Format"
 */
float calculate_humidity(char *data)
{
    // Extraire les 20 bits d'humidité
    // Attention: il faut cast en unsigned pour éviter les problèmes de signe
    uint32_t raw_humidity = 0;
    
    raw_humidity = ((uint32_t)(unsigned char)data[1] << 12) |  // Byte 1: bits 19-12
                   ((uint32_t)(unsigned char)data[2] << 4) |    // Byte 2: bits 11-4
                   ((uint32_t)(unsigned char)data[3] >> 4);     // Byte 3: bits 7-4 → 3-0
    
    // Formule: RH = (raw / 2^20) * 100
    float humidity = ((float)raw_humidity / 1048576.0) * 100.0;
    
    return humidity;
}

/*
 * Calcule la température en °C à partir des données brutes
 * 
 * Format des données AHT20:
 * [3]: Humidité [3:0] (bits 7:4) | Température [19:16] (bits 3:0)
 * [4]: Température [15:8]
 * [5]: Température [7:0]
 * 
 * Référence: AHT20 Datasheet section "Data Format"
 */
float calculate_temperature(char *data)
{
    // Extraire les 20 bits de température
    // Attention: cast en unsigned char pour éviter l'extension de signe
    uint32_t raw_temp = 0;
    
    raw_temp = (((uint32_t)(unsigned char)data[3] & 0x0F) << 16) |  // Byte 3: bits 3-0 → 19-16
               ((uint32_t)(unsigned char)data[4] << 8) |             // Byte 4: bits 15-8
               ((uint32_t)(unsigned char)data[5]);                   // Byte 5: bits 7-0
    
    // Formule: T = (raw / 2^20) * 200 - 50
    float temperature = ((float)raw_temp / 1048576.0) * 200.0 - 50.0;
    
    return temperature;
}
