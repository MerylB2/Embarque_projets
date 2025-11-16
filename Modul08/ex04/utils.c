/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 00:35:22 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/16 02:27:16 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void read_line(char *buffer, uint8_t max_len)
{
	uint8_t buf_idx = 0;
	char c;
	
	while (1)
	{
		c = uart_rx();
		
		// Backspace (DEL ou BS)
		if (c == 0x7F || c == 0x08)
		{
			if (buf_idx > 0)
			{
				buf_idx--;
				uart_tx('\b');  // Recule le curseur
				uart_tx(' ');   // Efface le caractère
				uart_tx('\b');  // Recule à nouveau
			}
			continue;
		}
		
		// Enter (CR ou LF)
		if (c == '\r' || c == '\n')
		{
			uart_tx('\r');
			uart_tx('\n');
			buffer[buf_idx] = '\0';
			break;
		}
		
		// Caractère normal
		if (buf_idx < max_len - 1)
		{
			buffer[buf_idx++] = c;
			uart_tx(c);  // Écho du caractère
		}
	}
}

char to_upper(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - 32;
	return c;
}

/*
** Convertit un nombre 0-15 en caractère hexadécimal majuscule
*/
char num_to_hex(uint8_t num)
{
	if (num < 10)
		return '0' + num;
	else
		return 'A' + (num - 10);
}

/*
** Convertit un caractère hexa en valeur numérique
** return: Valeur 0-15, ou 0xFF si invalide
*/
uint8_t hex_to_num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return 0xFF;  // Invalide
}

/*
** Parse une valeur hex de 2 caractères
** @param h: Caractère high nibble
** @param l: Caractère low nibble
** @return: Valeur 8 bits (0-255), ou 0xFF si invalide
*/
uint8_t parse_hex_byte(char h, char l)
{
	uint8_t high = hex_to_num(h);
	uint8_t low = hex_to_num(l);
	
	if (high == 0xFF || low == 0xFF)
		return 0xFF;
	
	return (high << 4) | low;
}

/*
** Affiche une valeur 8 bits en hexadécimal (2 caractères)
*/
void print_hex_byte(uint8_t value)
{
	uart_tx(num_to_hex(value >> 4));    // High nibble
	uart_tx(num_to_hex(value & 0x0F));  // Low nibble
}

uint8_t str_len(const char *str)
{
	uint8_t len = 0;
	while (str[len])
		len++;
	return len;
}

uint8_t str_cmp(const char *s1, const char *s2)
{
	uint8_t i = 0;
	while (s1[i] && s2[i])
	{
		if (to_upper(s1[i]) != to_upper(s2[i]))
			return 0;
		i++;
	}
	return s1[i] == s2[i];
}
