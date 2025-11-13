/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:04:30 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/12 20:00:43 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "aht20.h"

# define UART_BAUDRATE 115200


/* I2C */

// Initialise l'interface I2C/TWI du microcontrôleur
void i2c_init(void);

// Démarre une transmission I2C (condition START)
void i2c_start(void);

// Termine une transmission I2C (condition STOP)
void i2c_stop(void);

// Envoie un octet sur le bus I2C
void i2c_write(uint8_t data);

// Lit un octet avec ACK
uint8_t i2c_read_ack(void);

// Lit un octet avec NACK (dernier octet)
uint8_t i2c_read_nack(void);

// Retourne le status de l'I2C (TWSR masqué)
uint8_t i2c_get_status(void);


/* UART */

// Initialisation de l'UART
void uart_init(void);

// Transmission d'un caractère via UART
void uart_tx(char c);

// Conversion et affichage en hexadécimal
void uart_printhex(uint8_t value);

// Transmission d'une chaîne de caractères via UART
void uart_printstr(const char* str);

// Envoie une chaîne avec retour à la ligne
void uart_println(const char *str);

/* Autres */
void print_status(const char *msg);

// Réinitialise le module I2C (utile après erreurs)
void i2c_reset(void);

#endif