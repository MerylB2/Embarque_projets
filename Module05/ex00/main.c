/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:13:23 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/08 18:17:17 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "uart.h"

int main(void)
{
    uint8_t adc_value;

    uart_init();
    adc_init();
    
    while (1)
    {
        // read de la valeur du pot
        adc_value = adc_read();
        
        // print en hex
        uart_printhex(adc_value);
        uart_printstr("\r\n");

        _delay_ms(20);
    }
}
