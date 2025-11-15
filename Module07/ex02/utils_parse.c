/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 15:17:59 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 21:27:44 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


// Calcule la longueur d'une chaîne
uint8_t ft_strlen(const char *str)
{
	uint8_t len = 0;
	while (str[len])
		len++;
	return len;
}

// Compare deux chaînes (retourne 0 si égales)
int8_t ft_strcmp(const char *s1, const char *s2)
{
	uint8_t i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return s1[i] - s2[i];
}


// Afficher un octet en hexa
void print_hex_byte(uint8_t b)
{
	uart_printhex_lower(b);
}

// Afficher un caractère ASCII
void print_ascii_char(uint8_t c)
{
	if (c >= 0x20 && c <= 0x7E)
		uart_tx(c);
	else
	{
		uart_tx('.');
	}
}

// PRINT : hexdump format -C (seulement les données utiles)
void cmd_print(void)
{
	uint16_t addr = 0;
	
	while (addr < EEPROM_SIZE)
	{
		// Vérifier si la ligne contient des données
		uint8_t has_data = 0;
		for (uint8_t i = 0; i < 16; i++)
		{
			if (eeprom_read_byte((uint8_t*)(addr + i)) != 0xFF)
			{
				has_data = 1;
				break;
			}
		}
		
		// Si ligne vide (que des FF), arrêter l'affichage
		if (!has_data) {
			break;
		}
		
		// Afficher l'adresse
		uart_tx('0');
		uart_tx('0');
		uart_tx('0');
		uart_tx('0');
		uart_printhex_lower((uint8_t)(addr >> 8));
		uart_printhex_lower((uint8_t)(addr & 0xFF));
		uart_tx(' ');
		uart_tx(' ');
		
		// Afficher 16 octets en hexa
		uint8_t line[16];
		for (uint8_t i = 0; i < 16; i++) {
			line[i] = eeprom_read_byte((uint8_t*)(addr + i));
			print_hex_byte(line[i]);
			// Ajouter un espace après chaque groupe de 2 octets
			if (i % 2 == 1) {
				uart_tx(' ');
			}
		}
		
		// Afficher la partie ASCII
		uart_tx('|');
		for (uint8_t i = 0; i < 16; i++) {
			print_ascii_char(line[i]);
		}
		uart_tx('|');
		uart_tx('\r');
		uart_tx('\n');
		
		addr += 16;
	}
	uart_println("...");
}


// Lecture d'une ligne avec backspace
void read_line(char *buffer, uint8_t max_len)
{
	uint8_t buf_idx = 0;
	char c;
	
	while (1) {
		c = uart_rx();
		
		// Backspace
		if (c == 0x7F || c == 0x08) {
			if (buf_idx > 0) {
				buf_idx--;
				uart_tx('\b');
				uart_tx(' ');
				uart_tx('\b');
			}
			continue;
		}
		
		// Enter
		if (c == '\r' || c == '\n') {
			uart_tx('\r');
			uart_tx('\n');
			buffer[buf_idx] = '\0';
			break;
		}
		
		// Caractère normal
		if (buf_idx < max_len - 1) {
			buffer[buf_idx++] = c;
			uart_tx(c);
		}
	}
}

// Parser la commande
void parse_command(const char *buffer, char *cmd) {
	uint8_t i = 0;
	uint8_t j = 0;
	
	while (buffer[i] && buffer[i] != ' ' && j < 15) {
		cmd[j++] = buffer[i++];
	}
	cmd[j] = '\0';
}

// Parser READ clé
void parse_read_command(const char *buffer, char *key)
{
	uint8_t i = 0;
	
	while (buffer[i] && buffer[i] != ' ') i++;
	while (buffer[i] == ' ') i++;
	
	uint8_t j = 0;
	// Vérifier si la clé est entre guillemets
	if (buffer[i] == '"') {
		i++; // Sauter le guillemet ouvrant
		while (buffer[i] && buffer[i] != '"' && j < MAX_STRING_LEN) {
			key[j++] = buffer[i++];
		}
	} else {
		// Clé sans guillemets
		while (buffer[i] && buffer[i] != ' ' && j < MAX_STRING_LEN) {
			key[j++] = buffer[i++];
		}
	}
	key[j] = '\0';
}

// Parser WRITE "clé" "valeur"
void parse_write_command(const char *buffer, char *key, char *value)
{
	uint8_t i = 0;
	
	while (buffer[i] && buffer[i] != ' ') i++;
	while (buffer[i] == ' ') i++;
	
	// Extraire la clé entre guillemets
	if (buffer[i] == '"')
	{
		i++;
		uint8_t j = 0;
		while (buffer[i] && buffer[i] != '"' && j < MAX_STRING_LEN)
			key[j++] = buffer[i++];
		key[j] = '\0';
		if (buffer[i] == '"') i++;
	}
	
	while (buffer[i] == ' ') i++;
	
	// Extraire la valeur entre guillemets
	if (buffer[i] == '"')
	{
		i++;
		uint8_t j = 0;
		while (buffer[i] && buffer[i] != '"' && j < MAX_STRING_LEN)
			value[j++] = buffer[i++];
		value[j] = '\0';
	}
}

// Parser FORGET clé
void parse_forget_command(const char *buffer, char *key)
{
	uint8_t i = 0;
	
	while (buffer[i] && buffer[i] != ' ') i++;
	while (buffer[i] == ' ') i++;
	
	uint8_t j = 0;
	while (buffer[i] && buffer[i] != ' ' && j < MAX_STRING_LEN) {
		key[j++] = buffer[i++];
	}
	key[j] = '\0';
}

// Exécuter la commande
void execute_command(const char *buffer)
{
	char cmd[16];
	char key[MAX_STRING_LEN + 1];
	char value[MAX_STRING_LEN + 1];
	
	parse_command(buffer, cmd);
	
	if (ft_strcmp(cmd, "PRINT") == 0)
		cmd_print();
		
	else if (ft_strcmp(cmd, "CLEAR") == 0) {
		// Effacer toute l'EEPROM
		for (uint16_t i = 0; i < EEPROM_SIZE; i++) {
			eeprom_write_byte((uint8_t*)i, 0xFF);
		}
		uart_println("done");
	} 
	else if (ft_strcmp(cmd, "READ") == 0) {
		parse_read_command(buffer, key);
		cmd_read(key);
	}
	else if (ft_strcmp(cmd, "WRITE") == 0) {
		parse_write_command(buffer, key, value);
		cmd_write(key, value);
	}
	else if (ft_strcmp(cmd, "FORGET") == 0) {
		parse_forget_command(buffer, key);
		cmd_forget(key);
	}
}
