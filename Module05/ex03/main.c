/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:48:23 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/11 20:22:46 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

int main(void)
{
    int16_t temp;
    
    uart_init();
    adc_init();
    
    while (1)
    {
        temp = convert_to_celsius();

        uart_printstr(int_to_str(temp));
        uart_printstr("\r\n");
        
        _delay_ms(20);
    }
    
    return 0;
}
