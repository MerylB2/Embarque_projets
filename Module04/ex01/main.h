/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmetee-b <cmetee-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:48:45 by cmetee-b          #+#    #+#             */
/*   Updated: 2025/11/06 15:43:38 by cmetee-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "isr.h"

void	init_timer1_pwm();
void	init_timer0_interrupt();
void    update_pwm_duty();

#endif