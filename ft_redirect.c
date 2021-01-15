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

int		fd_check_and_dup(t_param *all)
{
	if (all->fd_1 != -1)
		dup2(all->fd_1, 1);
	if (all->fd_0 != -1)
		dup2(all->fd_0, 0);
	return (0);
}

int		fd_check_and_close(t_param *all)
{
	if (all->fd_1 != -1)
		all->fd_1 = fd_close(all->fd_1);
	if (all->fd_0 != -1)
		all->fd_0 = fd_close(all->fd_0);
	if (all->save_fd_1 != -1)
		all->save_fd_1 = fd_close(all->save_fd_1);
	if (all->save_fd_0 != -1)
		all->save_fd_0 = fd_close(all->save_fd_0);
	return (0);
}

int		fd_close(int fd)
{
	close(fd);
	fd = -1;
	return (fd);
}

int		cmd_remove_and_shift(t_param *all, int i)
{
	free(all->cmd[i]);
	while (all->cmd[i])
	{
		all->cmd[i] = all->cmd[i + 1];
		i++;
	}
	return (0);
}

void	fd_processor(t_param *all)
{
	int i;

	i = 0;
	while (all->cmd[i] != NULL)
	{
		if (all->cmd[i][0] == ';' || all->cmd[i][0] == '|')
		{
			ft_putendl("- ; or | -");
			break ;
		}

		if (all->fd_1 != -1 && ((all->cmd[i][0] == '>' && all->cmd[i][1] == '\0') ||
			(all->cmd[i][0] == '>' && all->cmd[i][1] == '>' && all->cmd[i][2] == '\0')))
			all->fd_1 = fd_close(all->fd_1);
		else if (all->fd_0 != -1 && (all->cmd[i][0] == '<' && all->cmd[i][2] == '\0'))
			all->fd_1 = fd_close(all->fd_1);

		if (all->cmd[i][0] == '>' && all->cmd[i][1] == '\0')
		{
			ft_putendl("->-");
			all->fd_1 = open(all->cmd[i + 1], O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);
			cmd_remove_and_shift(all, i);
			cmd_remove_and_shift(all, i);
			all->redirect = 1;
		}
		else if (all->cmd[i][0] == '>' && all->cmd[i][1] == '>' && all->cmd[i][2] == '\0')
		{
			ft_putendl("->>-");
			all->fd_1 = open(all->cmd[i + 1], O_RDWR | O_CREAT | O_APPEND, S_IWRITE | S_IREAD);
			cmd_remove_and_shift(all, i);
			cmd_remove_and_shift(all, i);
			all->redirect = 1;
		}
		else if (all->cmd[i][0] == '<' && all->cmd[i][1] == '\0')
		{
			ft_putendl("-<-");
			all->fd_0 = open(all->cmd[i + 1], O_RDONLY);
			cmd_remove_and_shift(all, i);
			cmd_remove_and_shift(all, i);
			all->redirect = 1;
		}
		else
			i++;
	}
}
