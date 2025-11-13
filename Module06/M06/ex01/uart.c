/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 15:33:44 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 15:24:50 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


void uart_init(void)
{
    uint16_t ubrr = (F_CPU / (8UL * BAUD)) - 1;
    
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);
    
    UCSR0A = (1 << U2X0);
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


void uart_tx(char c)
{

    while (!(UCSR0A & (1 << UDRE0)))
        ;

    UDR0 = c;
}


void uart_printstr(const char *str)
{
    while (*str)
        uart_tx(*str++);
}


void uart_printhex(uint8_t value)
{
    const char hex[] = "0123456789ABCDEF";
    
    uart_tx(hex[(value >> 4) & 0x0F]);   // Quartet de poids fort
    uart_tx(hex[value & 0x0F]);          // Quartet de poids faible
}


void uart_println(const char *str)
{
    uart_printstr(str);
    uart_tx('\r');
    uart_tx('\n');
}

void print_hex_value(char *c)
{
    const char hex[] = "0123456789ABCDEF";
    
    // Afficher les 7 octets
    for (uint8_t i = 0; i < 7; i++)
    {
        uart_tx(hex[(c[i] >> 4) & 0x0F]);
        uart_tx(hex[c[i] & 0x0F]);
        uart_tx(' ');
    }

    uart_tx('\r');
    uart_tx('\n');
}
