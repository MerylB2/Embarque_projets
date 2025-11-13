/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:04:30 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 17:18:00 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "aht20.h"

# define UART_BAUDRATE 115200

#define I2C_WRITE 0
#define I2C_READ  1

/* I2C */

// Initialise l'interface I2C/TWI du microcontrôleur
void i2c_init(void);

// Démarre une transmission I2C (condition START)
void i2c_start(void);

// Termine une transmission I2C (condition STOP)
void i2c_stop(void);

/* utiles pour faciliter la communication*/

// Écrit le contenu du registre TWDR et l'envoie au capteur
void i2c_write(unsigned char data);

// Affiche le contenu du registre TWDR après la mesure par le capteur (avec ACK)
void i2c_read(void);

// Lit le dernier octet avec NACK
void i2c_read_last(void);

// Retourne le status I2C
uint8_t i2c_get_status(void);


/* UART */

void uart_init(void);

void uart_tx(char c);

void uart_printhex(uint8_t value);

void uart_printstr(const char* str);

// Envoie une chaîne avec retour à la ligne
void uart_println(const char *str);

/* Écrit le contenu des 7 octets d'une mesure AHT20 en hexa */
void print_hex_value(char *c);

#endif