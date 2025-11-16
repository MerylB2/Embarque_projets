/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rgb.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 00:01:46 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/16 03:00:29 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void fullrainbow_mode(void)
{
	uart_printstr(" ✓ OK - Rainbow mode (press any key to exit)\r\n");
	
	/*
	** Vider le buffer UART avant de commencer
	** pour éviter qu'un ancien caractère n'arrête immédiatement l'animation
	*/
	while (uart_available())
		uart_rx();
	
	uint8_t pos = 0;
	
	// Boucle infinie jusqu'à interruption
	while (1)
	{
		wheel(pos, 0);                  // D6
		wheel((uint8_t)(pos + 85), 1);  // D7 décalé de 85
		wheel((uint8_t)(pos + 170), 2); // D8 décalé de 170
		
		pos++;  // Incrémente et déborde automatiquement à 0 après 255
		
		_delay_ms(10);
		
		/*
		** Vérifier si une touche a été pressée
		** Si oui, sortir du mode rainbow
		*/
		if (uart_available())
		{
			// Vider le buffer (lire et ignorer le caractère)
			while (uart_available())
				uart_rx();
			uart_printstr("Rainbow mode stopped\r\n");
			return;
		}
	}
}

/*
** Parse et exécute une commande reçue
*/
void process_command(char *cmd)
{
	// Vérifier que la commande commence par #
	if (cmd[0] != '#')
	{
		uart_printstr("ERROR: Command must start with #\r\n");
		return;
	}
	
	// Sauter le #
	cmd++;
	
	// Vérifier si c'est FULLRAINBOW
	if (str_cmp(cmd, "FULLRAINBOW"))
	{
		fullrainbow_mode();
		return;
	}
	
	/*
	** Format attendu: RRGGBBDX (8 caractères)
	** RR = rouge (2 hex)
	** GG = vert (2 hex)
	** BB = bleu (2 hex)
	** D = lettre 'D' (ou 'd')
	** X = chiffre 6, 7 ou 8
	*/
	
	// Vérifier qu'on a exactement 8 caractères
	uint8_t len = 0;
	while (cmd[len] != '\0' && len < 10)
		len++;
	
	if (len != 8)
	{
		uart_printstr("ERROR: Format must be #RRGGBBDX (8 chars)\r\n");
		return;
	}
	
	// Parser RR (rouge)
	uint16_t red_result = parse_hex_byte(cmd[0], cmd[1]);
	if (red_result > 255)
	{
		uart_printstr("ERROR: Invalid RED (chars 1-2 must be hex)\r\n");
		return;
	}
	uint8_t red = (uint8_t)red_result;
	
	// Parser GG (vert)
	uint16_t green_result = parse_hex_byte(cmd[2], cmd[3]);
	if (green_result > 255)
	{
		uart_printstr("ERROR: Invalid GREEN (chars 3-4 must be hex)\r\n");
		return;
	}
	uint8_t green = (uint8_t)green_result;
	
	// Parser BB (bleu)
	uint16_t blue_result = parse_hex_byte(cmd[4], cmd[5]);
	if (blue_result > 255)
	{
		uart_printstr("ERROR: Invalid BLUE (chars 5-6 must be hex)\r\n");
		return;
	}
	uint8_t blue = (uint8_t)blue_result;

	// Vérifier 'D' MAJUSCULE UNIQUEMENT (position 6)
	if (cmd[6] != 'D')
	{
		uart_printstr("ERROR: Must use 'D' (uppercase) at position 7\r\n");
		return;
	}
	
	// Parser le numéro de LED (position 7)
	char led_char = cmd[7];
	if (led_char < '6' || led_char > '8')
	{
		uart_printstr("ERROR: LED number must be 6, 7, or 8\r\n");
		return;
	}
	
	uint8_t led_index = led_char - '6';  // '6'->0, '7'->1, '8'->2
	
	// Appliquer la couleur
	rgb_set_led(led_index, red, green, blue);
	
	// Afficher la couleur au format #RRGGBBDX (D toujours en majuscule)
	uart_tx('#');
	print_hex_byte(red);
	print_hex_byte(green);
	print_hex_byte(blue);
	uart_tx('D');
	uart_tx(led_char); // 6, 7 ou 8
	uart_printstr(" : ✓ OK \r\n");
}

