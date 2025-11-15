/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:02:28 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/15 21:15:29 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int main(void)
{
    char buffer[128];
    uart_init();
    
    for (volatile uint32_t i = 0; i < 100000; i++);
    
    while (1)
    {
        uart_printstr("> ");  // Afficher le prompt
        read_line(buffer, sizeof(buffer));
        execute_command(buffer);
    }
    
    return 0;
}
