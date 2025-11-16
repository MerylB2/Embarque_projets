/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:34:13 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/16 01:58:09 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


int main(void)
{
	char buffer[20];  // Buffer pour la commande

	uart_init();
	spi_init();
	
	uart_printstr("\r\n=== WELCOME - IL-Series ===\r\n");
	uart_printstr("Commands:\r\n");
	uart_printstr("  #RRGGBBDX    - Set LED color (DX = D6/D7/D8)\r\n");
	uart_printstr("  #FULLRAINBOW - Rainbow effect\r\n\r\n");
	
	while (1)
	{
		uart_printstr("> ");
		read_line(buffer, sizeof(buffer));
		process_command(buffer);
	}
	
	return 0;
}
