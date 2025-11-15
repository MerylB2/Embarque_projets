/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:07:10 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 21:48:00 by cmetee-b         ###   ########.fr       */
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

// Magic byte pour identifier une paire valide (non-ASCII standard)
#define MAGIC_BYTE 0x7F

// Taille max des chaînes
#define MAX_STRING_LEN 32

/* UART */
void uart_init(void);
void uart_tx(char c);
void uart_printstr(const char *str);
void uart_printhex(uint8_t value);
void uart_printhex_lower(uint8_t value);
void uart_println(const char *str);
char uart_rx(void);

/* Commandes */
void cmd_read(const char *key);
void cmd_write(const char *key, const char *value);
void cmd_forget(const char *key);
void cmd_print(void);

/* Fonctions utilitaires */
uint16_t find_key(const char *key, uint16_t *data_addr);
uint16_t find_free_space(void);
void print_hex_byte(uint8_t b);
void print_ascii_char(uint8_t c);
uint8_t ft_strlen(const char *str);
int8_t ft_strcmp(const char *s1, const char *s2);

/* Parsing */
void read_line(char *buffer, uint8_t max_len);
void parse_command(const char *buffer, char *cmd);
void parse_read_command(const char *buffer, char *key);
void parse_write_command(const char *buffer, char *key, char *value);
void parse_forget_command(const char *buffer, char *key);
void execute_command(const char *buffer);

#endif
