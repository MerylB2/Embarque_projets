/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:13:17 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 17:43:20 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"


void uart_init(void)
{
    /* Calcul du baudrate en mode U2X pour meilleure précision
     * UBRR = (F_CPU / (8 * BAUDRATE)) - 1 (table 20-1  page 182)
     * Pour 16MHz et 115200 bauds : UBRR = 16 (erreur +2.1%) p.196 - ligne 4 table(20-4)
     */
    unsigned int ubrr = (F_CPU / (8UL * BAUD)) - 1;
    
    /* Activation du mode double vitesse U2X0 (20.3.2 - p.182)
     * Bit 1 – U2Xn: Double the USART Transmission Speed (p.201)
	 * Permet une meilleure précision du baud rate
	 */
    UCSR0A = (1 << U2X0);
    
    /* Configuration du baudrate (p.182) 
     * "UBRRnContents of the UBRRnH and UBRRnL Registers, (0-4095)"
     * voir aussi : USART initialization code example (20.5 - p 184-185)
     */
    UBRR0H = (unsigned int)(ubrr >> 8);
    UBRR0L = (unsigned int)(ubrr);
    
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
    while (!(UCSR0A & (1 << UDRE0)));
    
    UDR0 = c;
}

void uart_printstr(const char *str)
{
    while (*str)
        uart_tx(*str++);
}

// Conversion et affichage d'un nbr en décimal (0-1023 -> ADC 10 bits)
char* printdec(uint16_t value)
{
   static char str[5] = {0};                       // 4 caractères + '\0'
    
    // Extraire chaque chiffre
    str[0] = (value / 1000) % 10 + '0';             // Milliers
    str[1] = (value / 100) % 10 + '0';              // Centaines
    str[2] = (value / 10) % 10 + '0';               // Dizaines
    str[3] = (value % 10) + '0';                    // Unités
    str[4] = '\0';
    
    // Remplacer les zéros de tête par des espaces/vide
    // Ex: "0042" devient "  42"
    for (uint8_t i = 0; i < 3; i++)
    {
        if (str[i] == '0')
            str[i] = ' ';
        else
            break;  // Arrêter au premier chiffre non-zéro
    }
    
    return str;
}
