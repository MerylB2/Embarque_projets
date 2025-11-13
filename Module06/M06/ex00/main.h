/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:04:30 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 16:18:10 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <util/twi.h>

# define UART_BAUDRATE 115200

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


/* I2C */

// Initialise l'interface I2C/TWI du microcontrôleur
void i2c_init(void);

// Démarre une transmission I2C (condition START)
void i2c_start(void);

// Termine une transmission I2C (condition STOP)
void i2c_stop(void);

/* utiles pour faciliter la communication*/

// Envoie un octet sur le bus I2C
void i2c_write(uint8_t data);
// Retourne le status de l'I2C (TWSR masqué)
uint8_t i2c_get_status(void);


/* UART */

void uart_init(void);

void uart_tx(char c);

void uart_printhex(uint8_t value);

void uart_printstr(const char* str);

// Envoie une chaîne avec retour à la ligne
void uart_println(const char *str);

// Affiche le status I2C après chaque envoi de data
void print_status(const char *msg);

#endif