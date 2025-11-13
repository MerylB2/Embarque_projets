/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_debug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:52:19 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/13 16:54:59 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
 * i2c_start_debug - Affiche le status après un START
 * 
 * Status attendus:
 * - 0x08: START transmitted
 * - 0x10: Repeated START transmitted
 */
void i2c_start_debug(void)
{
    uint8_t status = TWSR & 0xF8;
    
    if (status == 0x08)
        uart_println("START condition transmitted");
    else if (status == 0x10)
        uart_println("Repeated START condition transmitted");
    else
    {
        uart_printstr("Unexpected Error - TWSR: ");
        uart_printhex(status);
        uart_tx('\r');
        uart_tx('\n');
    }
}

/*
 * i2c_write_address_debug - Debug après envoi SLA+W
 * 
 * Status attendus:
 * - 0x18: SLA+W transmitted, ACK received
 * - 0x20: SLA+W transmitted, NACK received
 * - 0x38: Arbitration lost
 */
void i2c_write_address_debug(void)
{
    uint8_t status = TWSR & 0xF8;
    
    uart_printstr("SLA+W transmitted - ");
    
    if (status == 0x18)
        uart_println("ACK received");
    else if (status == 0x20)
        uart_println("NACK received");
    else if (status == 0x38)
        uart_println("Arbitration lost");
    else
    {
        uart_printstr("Unexpected Error - TWSR: ");
        uart_printhex(status);
        uart_tx('\r');
        uart_tx('\n');
    }
}

/*
 * i2c_read_address_debug - Debug après envoi SLA+R
 * 
 * Status attendus:
 * - 0x40: SLA+R transmitted, ACK received
 * - 0x48: SLA+R transmitted, NACK received
 * - 0x38: Arbitration lost
 */
void i2c_read_address_debug(void)
{
    uint8_t status = TWSR & 0xF8;
    
    uart_printstr("SLA+R transmitted - ");
    
    if (status == 0x40)
        uart_println("ACK received");
    else if (status == 0x48)
        uart_println("NACK received");
    else if (status == 0x38)
        uart_println("Arbitration lost");
    else
    {
        uart_printstr("Unexpected Error - TWSR: ");
        uart_printhex(status);
        uart_tx('\r');
        uart_tx('\n');
    }
}

/*
 * i2c_write_debug - Debug après envoi d'une donnée
 * 
 * Status attendus:
 * - 0x28: Data transmitted, ACK received
 * - 0x30: Data transmitted, NACK received
 * - 0x38: Arbitration lost
 */
void i2c_write_debug(void)
{
    uint8_t status = TWSR & 0xF8;
    
    uart_printstr("Data byte transmitted - ");
    
    if (status == 0x28)
        uart_println("ACK received");
    else if (status == 0x30)
        uart_println("NACK received");
    else if (status == 0x38)
        uart_println("Arbitration lost");
    else
    {
        uart_printstr("Unexpected Error - TWSR: ");
        uart_printhex(status);
        uart_tx('\r');
        uart_tx('\n');
    }
}

/*
 * i2c_read_debug - Debug après lecture d'une donnée
 * 
 * Status attendus:
 * - 0x50: Data received, ACK returned
 * - 0x58: Data received, NACK returned
 */
void i2c_read_debug(uint8_t data)
{
    uint8_t status = TWSR & 0xF8;
    
    if (status == 0x50 || status == 0x58)
    {
        uart_printstr("Data received: ");
        uart_printhex(data);
        
        if (status == 0x50)
            uart_println(" - ACK returned");
        else
            uart_println(" - NACK returned");
    }
    else
    {
        uart_printstr("Unexpected Error - TWSR: ");
        uart_printhex(status);
        uart_tx('\r');
        uart_tx('\n');
    }
}
