/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 20:50:08 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/13 20:50:21 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		fd_close(int fd)
{
	close(fd);
	fd = -1;
	return (fd);
}

void	fd_processor(t_param *all)
{
	int i;

	i = 0;
	while (all->cmd[i] != NULL || all->cmd[i][0] != ';' || all->cmd[i][0] != '|')
	{

		if (all->fd_1 != -1 && (all->cmd[i][0] == '>' || (all->cmd[i][0] == '>' && all->cmd[i][1] == '>')))
			all->fd_1 = fd_close(all->fd_1);
		else if (all->fd_0 != -1 && (all->cmd[i][0] == '<'))
			all->fd_1 = fd_close(all->fd_1);

		if (all->cmd[i][0] == '>')
			all->fd_1 = open(all->cmd[++i], O_RDWR | O_CREAT | \
			O_TRUNC, S_IWRITE | S_IREAD);
		else if (all->cmd[i][0] == '>' && all->cmd[i][1] == '>')
			all->fd_1 = open(all->cmd[++i], O_RDWR | O_CREAT | \
			O_APPEND, S_IWRITE | S_IREAD);

		else if (all->cmd[i][0] == '<')
			all->fd_0 = open(all->cmd[++i], O_RDONLY);

		i++;
	}
}
