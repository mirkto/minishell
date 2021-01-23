/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 05:10:27 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/11 19:21:32 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler_int_c(int i)
{
	i = 0;
	write(1, "\b\b  \b\b", 6);
	write(1, "\n", 1);
	write(1, "\033[0;32mminishell$ \033[0m", 22);
}

void	handler_int_c_2(int i)
{
	i = 0;
	write(1, "\n", 1);
}

void	handler_quit_(int i)
{
	i = 0;
	write(1, "\b\b  \b\b", 6);
}
