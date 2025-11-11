/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:13:23 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 16:56:44 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "uart.h"

int main(void)
{
    uint16_t adc_value;

    uart_init();
    adc_init();
    
    while (1)
    {
        adc_value = adc_read(0);
        uart_printstr(printdec(adc_value));
        uart_printstr(", ");
        
        adc_value = adc_read(1);
        uart_printstr(printdec(adc_value));
        uart_printstr(", ");
        
        adc_value = adc_read(2);
        uart_printstr(printdec(adc_value));
        uart_printstr("\r\n");

        _delay_ms(20);
    }
}
