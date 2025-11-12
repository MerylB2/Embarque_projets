/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aht20.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:00:46 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/12 18:05:38 by cmetee-b         ###   ########.fr       */
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
#define AHT20_CMD_INIT    0xBE  // 1011 1110 - Initialization
#define AHT20_CMD_TRIGGER 0xAC  // 1010 1100 - Trigger Measurement
#define AHT20_CMD_RESET   0xBA  // 1011 1010 - Soft reset

/*
 * Status bits
 * Référence: AHT20 Datasheet Table 10 "Status bit description"
 */
#define AHT20_STATUS_BUSY        (1 << 7)  // Bit[7]: 1=busy, 0=free
#define AHT20_STATUS_CALIBRATED  (1 << 3)  // Bit[3]: 1=calibrated, 0=uncalibrated

/* Structure pour stocker les données du capteur */
typedef struct
{
    float temperature;  // Température en °C
    float humidity;     // Humidité relative en %
    uint8_t status;     // Byte de status brut
} aht20_data_t;

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
void aht20_read_raw_data(uint8_t *data);

/*
 * Convertit les données brutes en température et humidité
 * 
 * Référence: AHT20 Datasheet section 5.4 Sensor reading process p.8 : step 5
 * "Calculate the temperature and humidity values"
 */
void aht20_convert_data(uint8_t *raw_data, aht20_data_t *result);

/*
 * Lecture complète : trigger + wait + read + convert
 * Fonction tout-en-un pour simplifier l'utilisation
 */
void aht20_read(aht20_data_t *result);

/*
 * Vérifie si le capteur est busy
 * Retourne 1 si busy, 0 si libre
 * 
 * Référence: AHT20 Datasheet Table 10
 * "Bit[7] Busy indication: 1=Busy in measurement, 0=Free in dormant state"
 */
uint8_t aht20_is_busy(uint8_t status);

/*
 * Vérifie si le capteur est calibré
 * Retourne 1 si calibré, 0 sinon
 * 
 * Référence: AHT20 Datasheet Table 10
 * "Bit[3] CAL Enable: 1=calibrated, 0=uncalibrated"
 */
uint8_t aht20_is_calibrated(uint8_t status);

#endif