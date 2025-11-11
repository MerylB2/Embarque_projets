/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:58:36 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 18:39:53 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

// Datasheet ATmega328P : Section 20 - USART
void uart_init(void)
{
    /* Calcul du baudrate en mode U2X
     * UBRR = (F_CPU / (8 * BAUDRATE)) - 1
     * Pour 16MHz et 115200 bauds : UBRR = 16 (erreur +2.1%)
     */
    unsigned int ubrr = (F_CPU / (8UL * UART_BAUDRATE)) - 1;
    
    UCSR0A = (1 << U2X0);                           // Mode double vitesse (p.182)
    UBRR0H = (unsigned int)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;                   // Baudrate (p.183)
    UCSR0B = (1 << TXEN0);                          // Activer TX (p.182)
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);         // Format 8N1 (p.184)
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0)));               // Attendre buffer vide
    UDR0 = c;
}

void uart_printstr(const char *str)
{
    while (*str)
        uart_tx(*str++);
}

char* int_to_str(int16_t value)
{
    static char str[7];  // "-32768\0" max
    uint8_t i = 0;
    uint8_t is_neg = 0;
    
    // Gérer les nombres négatifs
    if (value < 0)
    {
        is_neg = 1;
        value = -value;
    }
    
    // Cas spécial : 0
    if (value == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    
    // Conversion en caractères (de droite à gauche)
    while (value > 0)
    {
        str[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    // Ajouter le signe si nécessaire
    if (is_neg)
        str[i++] = '-';
    
    str[i] = '\0';
    
    // Inverser la string
    for (uint8_t j = 0; j < i / 2; j++)
    {
        char temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }
    
    return str;
}

