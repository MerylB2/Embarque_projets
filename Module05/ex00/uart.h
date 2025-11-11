/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:18:19 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/07 18:26:21 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

# define UART_BAUDRATE 115200

/* Initialisation de l'UART */
void uart_init(void);

/* Transmission d'un caractère via UART */
void uart_tx(char c);

/* Conversion et affichage en hexadécimal */
void uart_printhex(uint8_t value);

/* Transmission d'une chaîne de caractères via UART */
void uart_printstr(const char* str);

/* Initialisation de l'ADC */
void adc_init(void);

/* Lecture de l'ADC */
uint8_t adc_read(void);

#endif