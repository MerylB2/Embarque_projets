/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aht20.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:00:46 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/12 20:43:33 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AHT20_H
#define AHT20_H

#include <stdint.h>

/* 
 * Adresse I2C de l'AHT20 (7 bits)
 * Référence: AHT20 Datasheet section 5.3 "Send Command" p.8
 */
#define AHT20_ADDR 0x38

/*
 * Commandes AHT20
 * Référence: AHT20 Datasheet Table 9 "Basic Commands" p.8
 */
#define AHT20_INIT_CMD    0xBE  // 1011 1110 - Initialization
#define AHT20_TRIGGER_CMD 0xAC  // 1010 1100 - Trigger Measurement


/*
 * Initialise le capteur AHT20
 * Vérifie la calibration et envoie la commande d'init si nécessaire
 * 
 * Référence: AHT20 Datasheet section 5.4 Sensor reading process p.8 : step 1
 * "Wait 40ms after power-on. Before reading the temperature and humidity 
 *  values, first check whether the calibration enable bit Bit[3] of the 
 *  status word is 1"
 */
void aht20_init(void);

/*
 * Déclenche une mesure de température et d'humidité
 * 
 * Référence: AHT20 Datasheet section 5.4 Sensor reading process p.8 : step 2
 * "Send the 0xAC command directly (trigger measurement)"
 */
void aht20_trigger_measurement(void);

/*
 * Lit les données brutes (7 octets) du capteur
 * 
 * Référence: AHT20 Datasheet section 5.4 Sensor reading process p.8 : step 4
 * "After receiving six bytes, the next byte is the CRC check data"
 */
void aht20_read_data(void);


#endif