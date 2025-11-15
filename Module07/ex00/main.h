/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:07:10 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 13:24:19 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/eeprom.h>

#define F_CPU 16000000UL

// Taille EEPROM ATmega328P - Section 7.4 EEPROM Data Memory p.19 (1KB)
#define EEPROM_SIZE 1024

/* UART */
void uart_init(void);
void uart_tx(char c);
void uart_printstr(const char *str);
void uart_printhex(uint8_t value);
void uart_printhex_lower(uint8_t value);
void uart_println(const char *str);

/* autres */
void eeprom_hexdump(void);

#endif