/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aht20.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:36:07 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/12 18:19:24 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

// https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf

/*
 * aht20_init - Initialise le capteur AHT20
 * 
 * Séquence d'initialisation complète selon le datasheet:
 * 
 * 1. Attendre 40ms après le power-on (requis par le capteur)
 * 2. Lire le status byte pour vérifier le bit de calibration Bit[3]
 * 3. Si Bit[3]=0 (non calibré), envoyer la commande d'initialisation 0xBE
 * 
 * Référence: AHT20 Datasheet section 5.4 "Sensor reading process" step 1
 * "Wait 40ms after power-on. Before reading the temperature and humidity 
 *  values, first check whether the calibration enable bit Bit[3] of the 
 *  status word is 1 (you can get a byte of status word by sending 0x71). 
 *  If not 1, need to send 0x0be command (initialization), this command 
 *  parameter has two bytes, the first byte is 0x08, the second byte is 0x00, 
 *  and wait for 10ms."
 * 
 * Note: Cette vérification est nécessaire au power-on uniquement.
 *       Pas besoin de la faire avant chaque mesure.
 */
void aht20_init(void)
{
    uint8_t status;
    
    // Step 1: Attendre 40ms après le power-on (requis par le hardware)
    _delay_ms(40);
    
    // Step 2: Lire le status byte pour vérifier la calibration
    // Séquence I2C: START → Address+Read → Read 1 byte → STOP
    i2c_start();
    i2c_write((AHT20_ADDR << 1) | 1);   // Adresse + Read bit
    status = i2c_read_nack();           // Lire le status (un seul byte, donc NACK)
    i2c_stop();
    
    // Step 3: Vérifier le bit de calibration Bit[3]
    // Table 10: "Bit[3] CAL Enable: 1=calibrated, 0=uncalibrated"
    if (!(status & AHT20_STATUS_CALIBRATED)) 
    {
        // Capteur non calibré, envoyer la commande d'initialisation
        // Commande: 0xBE 0x08 0x00
        // Table 9: "Initialization - Keep main engine - 1011 1110"
        
        i2c_start();
        i2c_write((AHT20_ADDR << 1) | 0);   // Adresse + Write bit
        i2c_write(AHT20_CMD_INIT);          // 0xBE - Commande init
        i2c_write(0x08);                    // Premier paramètre
        i2c_write(0x00);                    // Deuxième paramètre
        i2c_stop();
        
        // Attendre 10ms après l'initialisation (requis par le datasheet)
        _delay_ms(10);
    }
}

/*
 * aht20_trigger_measurement - Déclenche une nouvelle mesure
 * 
 * Envoie la commande 0xAC (Trigger Measurement) avec ses paramètres.
 * La mesure prend environ 80ms à compléter.
 * 
 * Commande complète: 0xAC 0x33 0x00
 * - 0xAC : commande "Trigger Measurement"
 * - 0x33 : premier paramètre
 * - 0x00 : deuxième paramètre
 * 
 * Référence: AHT20 Datasheet section 5.4 step 2
 * "Send the 0xAC command directly (trigger measurement). 
 *  The parameter of this command has two bytes, the first byte is 0x33 
 *  and the second byte is 0x00."
 * 
 * Table 9: "Trigger Measurement - Keep main engine - 1010 1100"
 */
void aht20_trigger_measurement(void)
{
    // Séquence I2C: START → Address+Write → 0xAC → 0x33 → 0x00 → STOP
    i2c_start();
    i2c_write((AHT20_ADDR << 1) | 0);   // Adresse + Write bit
    i2c_write(AHT20_CMD_TRIGGER);       // 0xAC - Commande trigger
    i2c_write(0x33);                    // Premier paramètre (fixe)
    i2c_write(0x00);                    // Deuxième paramètre (fixe)
    i2c_stop();
}

/*
 * aht20_read_raw_data - Lit les 7 octets de données brutes
 * 
 * Format des 7 octets (voir diagrammes dans section 5.4):
 * 
 * Byte 0: Status byte
 *   - Bit[7]: Busy (1=measurement in progress, 0=free)
 *   - Bit[3]: Calibrated (1=yes, 0=no)
 * 
 * Byte 1-3: Données d'humidité (20 bits)
 *   - Byte 1: bits [19:12] de l'humidité
 *   - Byte 2: bits [11:4] de l'humidité
 *   - Byte 3[7:4]: bits [3:0] de l'humidité
 * 
 * Byte 3-5: Données de température (20 bits)
 *   - Byte 3[3:0]: bits [19:16] de la température
 *   - Byte 4: bits [15:8] de la température
 *   - Byte 5: bits [7:0] de la température
 * 
 * Byte 6: CRC (optionnel)
 *   - Formule: CRC[7:0] = 1 + x^4 + x^5 + x^8
 * 
 * Référence: AHT20 Datasheet section 5.4 step 4
 * "After receiving six bytes, the next byte is the CRC check data, 
 *  the user can read it as needed, if the receiving end needs CRC check, 
 *  CRC initial value is 0xFF, CRC8 check polynomial is: 
 *  CRC[7:0] = 1 + x^4 + x^5 + x^8"
 * 
 * Les diagrammes montrent l'organisation des bits:
 * - "Read temperature and humidity data" (diagrammes bleus)
 * - Les octets sont lus avec ACK sauf le dernier (NACK)
 */
void aht20_read_raw_data(uint8_t *data)
{
    // Séquence I2C: START → Address+Read → Read 6 bytes (ACK) → Read 1 byte (NACK) → STOP
    i2c_start();
    i2c_write((AHT20_ADDR << 1) | 1);  // Adresse + Read bit
    
    // Lire les 6 premiers octets avec ACK (on veut continuer)
    for (uint8_t i = 0; i < 6; i++) {
        data[i] = i2c_read_ack();
    }
    
    // Lire le 7ème octet (CRC) avec NACK (dernier octet)
    data[6] = i2c_read_nack();
    
    i2c_stop();
}

/*
 * aht20_convert_data - Convertit les données brutes en valeurs réelles
 * 
 * Extraction des données (voir diagrammes dans la documentation):
 * 
 * HUMIDITÉ (20 bits):
 * ┌──────────┬──────────┬────────────┐
 * │  Byte 1  │  Byte 2  │  Byte 3    │
 * │ [19:12]  │ [11:4]   │[7:4]→[3:0] │
 * └──────────┴──────────┴────────────┘
 * 
 * TEMPÉRATURE (20 bits):
 * ┌──────────┬──────────┬──────────┐
 * │ Byte 3   │  Byte 4  │  Byte 5  │
 * │[3:0]→[19:16]│ [15:8]│  [7:0]   │
 * └──────────┴──────────┴──────────┘
 * 
 * Formules de conversion (section 5.4 step 5):
 * 
 * Humidité (%RH) = (S_RH / 2^20) × 100
 * Température (°C) = (S_T / 2^20) × 200 - 50
 * 
 * Où:
 * - S_RH = valeur brute de l'humidité (20 bits)
 * - S_T = valeur brute de la température (20 bits)
 * - 2^20 = 1048576
 * 
 * Référence: AHT20 Datasheet section 5.4 step 5
 * "Calculate the temperature and humidity values"
 * Note en bas: "The calibration status check in the first step only needs 
 * to be checked at power-on. No operation is required during the normal 
 * acquisition process."
 */
void aht20_convert_data(uint8_t *raw_data, aht20_data_t *result)
{
    uint32_t humidity_raw;
    uint32_t temperature_raw;
    
    // Sauvegarder le status byte
    result->status = raw_data[0];
    
    // Extraction de l'humidité (20 bits)
    // Byte 1: bits [19:12]
    // Byte 2: bits [11:4]
    // Byte 3 (partie haute): bits [3:0]
    humidity_raw = ((uint32_t)raw_data[1] << 12) |   // Byte 1 → bits [19:12]
                   ((uint32_t)raw_data[2] << 4) |    // Byte 2 → bits [11:4]
                   ((uint32_t)raw_data[3] >> 4);     // Byte 3 (high nibble) → bits [3:0]
    
    // Extraction de la température (20 bits)
    // Byte 3 (partie basse): bits [19:16]
    // Byte 4: bits [15:8]
    // Byte 5: bits [7:0]
    temperature_raw = (((uint32_t)raw_data[3] & 0x0F) << 16) |  // Byte 3 (low nibble) → bits [19:16]
                      ((uint32_t)raw_data[4] << 8) |             // Byte 4 → bits [15:8]
                      ((uint32_t)raw_data[5]);                   // Byte 5 → bits [7:0]
    
    // Conversion en valeurs réelles selon les formules du datasheet
    // Humidité: (S_RH / 2^20) × 100
    // Note: 1048576 = 2^20
    result->humidity = (humidity_raw / 1048576.0f) * 100.0f;
    
    // Température: (S_T / 2^20) × 200 - 50
    result->temperature = (temperature_raw / 1048576.0f) * 200.0f - 50.0f;
}

/*
 * aht20_read - Fonction tout-en-un pour lire température et humidité
 * 
 * Séquence complète:
 * 1. Déclencher la mesure (0xAC)
 * 2. Attendre 80ms pour que la mesure se termine
 * 3. Lire les 7 octets de données
 * 4. Convertir en valeurs réelles
 * 
 * Référence: AHT20 Datasheet section 5.4 step 3
 * "Wait for 80ms to wait for the measurement to be completed. 
 *  If the read status word Bit[7] is 0, it indicates that the measurement 
 *  is completed, and then six bytes can be read in a row; otherwise, 
 *  continue to wait."
 * 
 * Note: On attend systématiquement 80ms au lieu de faire un polling du 
 * bit Busy, car c'est plus simple et évite des communications I2C supplémentaires.
 */
void aht20_read(aht20_data_t *result)
{
    uint8_t raw_data[7];
    
    // Step 1: Déclencher la mesure
    aht20_trigger_measurement();
    
    // Step 2: Attendre que la mesure soit complète
    // Le datasheet indique 80ms minimum
    _delay_ms(80);
    
    // Step 3: Lire les données brutes (7 octets)
    aht20_read_raw_data(raw_data);
    
    // Step 4: Convertir en température et humidité
    aht20_convert_data(raw_data, result);
}

/*
 * aht20_is_busy - Vérifie si une mesure est en cours
 * 
 * Retourne 1 si le capteur est occupé (mesure en cours)
 * Retourne 0 si le capteur est libre (dormant)
 * 
 * Référence: AHT20 Datasheet Table 10 "Status bit description"
 * "Bit[7] Busy indication:
 *  1 = Busy in measurement
 *  0 = Free in dormant state"
 */
uint8_t aht20_is_busy(uint8_t status)
{
    return (status & AHT20_STATUS_BUSY) ? 1 : 0;
}

/*
 * aht20_is_calibrated - Vérifie si le capteur est calibré
 * 
 * Retourne 1 si le capteur est calibré
 * Retourne 0 si le capteur n'est pas calibré
 * 
 * Référence: AHT20 Datasheet Table 10 "Status bit description"
 * "Bit[3] CAL Enable:
 *  1 = calibrated
 *  0 = uncalibrated"
 */
uint8_t aht20_is_calibrated(uint8_t status)
{
    return (status & AHT20_STATUS_CALIBRATED) ? 1 : 0;
}