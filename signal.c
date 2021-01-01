/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 05:10:27 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/01 05:10:30 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler_int_c(int i)		//signal(SIGINT, handler_int_c);
{
	i = 0;
	write(1, "\b\b  \b\b\n", 7);
	write(1, "\033[0;32mminishell-0.3$ \033[0m", 26);
}

void	handler_kill_d(int i)	//signal(SIGKILL, handler_kill_d);
{
	i = 0;
	write(1, "exit\n", 5);
}

void	handler_quit_(int i)		//signal(SIGQUIT, handler_quit_);
{
	i = 0;
	write(1, "\b\b  \b\b", 6);
}
