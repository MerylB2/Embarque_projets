/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:13:17 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/07 19:57:37 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"


void uart_init(void)
{
    /* Calcul du baudrate en mode U2X pour meilleure précision
     * UBRR = (F_CPU / (8 * BAUDRATE)) - 1 (table 20-1  page 182)
     * Pour 16MHz et 115200 bauds : UBRR = 16 (erreur +2.1%) p.196 - ligne 4 table(20-4)
     */
    uint16_t ubrr = (F_CPU / (8UL * BAUD)) - 1;
    
    /* Configuration du baud rate (p.182) 
     * "UBRRnContents of the UBRRnH and UBRRnL Registers, (0-4095)"
     * voir aussi : USART initialization code example (20.5 - p 184-185)
     */
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);
    
    /* Activation du mode double vitesse U2X0 (20.3.2 - p.182)
     * Bit 1 – U2Xn: Double the USART Transmission Speed (p.201)
	 * Permet une meilleure précision du baud rate
	 */
    UCSR0A = (1 << U2X0);
    
    /* Activation du transmetteur (20.6 - p.185-186)
     * 20.11.3 UCSRnB – USART Control and Status Register n B (p.201-202)
	 * bit 3 – TXEN0: Transmitter Enable
	 */
    UCSR0B = (1 << TXEN0);
    
    /*Configuration du format: 8 bits, 1 stop bit, pas de parité (p.186-187)
	 * UCSZ01:UCSZ00 = 11 pour 8 bits de données (Table 20-11 p.203)
     */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c)
{
    /* 20.11.2 - p.200
	 * Attente que le buffer de transmission soit vide (p.200)
	 * UDRE0 (bit 5) = 1 quand UDR0 est prêt à recevoir des données
	 */
    while (!(UCSR0A & (1 << UDRE0)));
    
    UDR0 = c;
}

void uart_printstr(const char *str)
{
    while (*str)
        uart_tx(*str++);
}

void uart_printhex(uint8_t value)
{
    const char hex[] = "0123456789abcdef";
    
    /* Afficher le digit de poids fort (MSB - 4 bits hauts)
     * value >> 4 : décale de 4 bits à droite pour isoler les 4 bits hauts
     * & 0x0F : masque pour garder seulement les 4 bits bas
     * Exple : 0xA1 >> 4 = 0x0A, hex[10] = 'a'
     */
    uart_tx(hex[(value >> 4) & 0x0F]);
    
    /* Afficher le digit de poids faible (LSB - 4 bits bas)
     * & 0x0F : masque pour garder seulement les 4 bits bas
     * Exple : 0xA1 & 0x0F = 0x01, hex[1] = '1'
     */
    uart_tx(hex[value & 0x0F]);
}
