/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 20:45:26 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/13 20:45:29 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fd_connect(int prev_fd[], int next_fd[])
{
	if (prev_fd[0] >= 0)
	{
		dup2(prev_fd[0], 0);
		close(prev_fd[0]);
		close(prev_fd[1]);
	}
	if (next_fd[1] >= 0)
	{
		dup2(next_fd[1], 1);
		close(next_fd[1]);
		close(next_fd[0]);
	}
}

int		pipe_conveyor(t_param *all)
{
	int		i;
	int		prev_fd[2];
	int		next_fd[2];

	next_fd[0] = -2;
	next_fd[1] = -2;
	i = 0;
	while (i <= all->pipe_num)
	{
		split_by_pipes(all);
		prev_fd[0] = next_fd[0];
		prev_fd[1] = next_fd[1];
		if (i != all->pipe_num)
			pipe(next_fd);
		else
		{
			next_fd[0] = -2;
			next_fd[1] = -2;
		}
		if (fork() == 0)
		{
			fd_connect(prev_fd, next_fd);
			executor(all);
			exit(1);
		}
		close(prev_fd[0]);
		close(prev_fd[1]);
		i++;
	}
	i = 0;
	while (i <= all->pipe_num)
	{
		wait(NULL);
		i++;
	}
	return (0);
}
