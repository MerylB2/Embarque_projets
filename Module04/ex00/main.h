/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:48:45 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/06 15:14:09 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void init_led(void);
void setup_sw_interrupt(void);

#endif