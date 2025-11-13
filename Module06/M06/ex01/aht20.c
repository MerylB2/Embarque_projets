/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aht20.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:36:07 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 17:41:51 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf

/*
 * Séquence d'initialisation du capteur AHT20
 * 
 * Séquence (Datasheet AHT20 section 5.4 step 1):
 * 1. Attendre 40ms après power-on (fait dans main) 
 * 2. Envoyer les cmds 0xBE 0x08 0x00
 * 3. Attendre 10ms
 */
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

/*
 * aht20_trigger_measurement - Déclenche une mesure
 * 
 * Séquence (Datasheet AHT20 section 5.4 step 2):
 * 1. Envoyer commande 0xAC 0x33 0x00
 * 2. Attendre 80ms minimum pour la mesure
 */
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
