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

int		check_back_slash_n(t_param *all, char *tmp)
{
	int i;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\n')
			return (0);
		i++;
	}
	all->tmp = ft_strdup(all->buf);
	free(all->buf);
	return (1);
}

char	*control_d_processing(t_param *all, char *tmp, char *buf, int flag)
{
	if (flag == 1)
	{
		free(tmp);
		tmp = ft_strdup(buf);
	}
	else if (flag == 2)
	{
		all->tmp_b = tmp;
		free(tmp);
		tmp = ft_strjoin(buf, all->tmp_b);
	}
	else
		return (NULL);
	free(buf);
	return (tmp);
}
