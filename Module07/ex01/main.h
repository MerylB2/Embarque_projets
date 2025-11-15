/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:07:10 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 13:36:51 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/eeprom.h>

#define F_CPU 16000000UL

// Codes ANSI pour couleurs
#define RED     "\033[31m"
#define RESET   "\033[0m"

// Taille EEPROM ATmega328P - Section 7.4 EEPROM Data Memory p.19 (1KB)
#define EEPROM_SIZE 1024

/* UART */
void uart_init(void);
void uart_tx(char c);
void uart_printstr(const char *str);
void uart_printhex(uint8_t value);
void uart_printhex_lower(uint8_t value);
void uart_println(const char *str);
char uart_rx(void);

/* autres EEPROM & Parsing */
void eeprom_hexdump(void);
void eeprom_hexdump_redlight(uint16_t highlight_addr);
uint16_t parse_hex_address(const char *str);
uint8_t parse_hex_byte(const char *str);

#endif