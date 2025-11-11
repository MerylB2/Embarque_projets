/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:18:19 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 20:19:14 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

# define UART_BAUDRATE 115200

void uart_init(void);

void uart_tx(char c);

void uart_printstr(const char* str);

/* Conversion d'un nombre signé en string (pour temp négative) */
char* int_to_str(int16_t value);

/* ADC */
void adc_init(void);

/* Lecture brute du capteur de température */
uint16_t adc_read_temp(void);

/* Conversion en degrés Celsius */
int16_t convert_to_celsius(void);

#endif