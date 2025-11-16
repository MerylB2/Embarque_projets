/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:02:47 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/16 01:31:03 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void uart_init(void)
{
	unsigned int ubrr = (F_CPU / (8UL * UART_BAUDRATE)) - 1;
	

	UCSR0A = (1 << U2X0);

	UBRR0H = (unsigned int)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	

	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

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

char uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)))
		;
	
	return UDR0;
}

uint8_t uart_available(void)
{
	/*
	** Vérifie si des données sont disponibles
	** Section 20.11.2 - Bit 7 RXC0: Receive Complete
	** Retourne 1 si un caractère est disponible, 0 sinon
	*/
	return (UCSR0A & (1 << RXC0)) != 0;
}