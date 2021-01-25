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

int		split_by_pipes(t_param *all)
{
	int		i;
	int		j;

	i = 0;
	while (all->pipes_remnant[i] != NULL)
	{
		if (!ft_strcmp(all->pipes_remnant[i], "|"))
		{
			all->pipes_remnant = split_loop(all, i, all->pipes_remnant);
			return (1);
		}
		i++;
	}
	free_array(all->cmd);
	all->cmd = copy_env(all->pipes_remnant, 0);
	j = 0;
	while (all->cmd[i + j])
		free(all->cmd[i + j++]);
	all->cmd[i] = NULL;
	free_array(all->pipes_remnant);
	return (0);
}

void	connect_and_execut(t_param *all, int prev_fd[], int next_fd[])
{
	all->input_fd_1 = dup(1);
	all->output_fd_0 = dup(0);
	all->pipes_fd = 1;
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
	executor(all);
	all->pipes_fd = 0;
	all->input_fd_1 = fd_close(all->input_fd_1);
	all->output_fd_0 = fd_close(all->output_fd_0);
	exit(1);
}

void	wait_all_execve(int number_of_pipes)
{
	int	i;

	i = 0;
	while (i <= number_of_pipes)
	{
		wait(NULL);
		i++;
	}
}

int		pipe_conveyor(t_param *all)
{
	int		prev_fd[2];
	int		next_fd[2];

	next_fd[0] = -2;
	next_fd[1] = -2;
	all->i = -1;
	while (++all->i <= all->pipe_num)
	{
		split_by_pipes(all);
		prev_fd[0] = next_fd[0];
		prev_fd[1] = next_fd[1];
		if (all->i != all->pipe_num)
			pipe(next_fd);
		else
		{
			next_fd[0] = -2;
			next_fd[1] = -2;
		}
		if (fork() == 0)
			connect_and_execut(all, prev_fd, next_fd);
		close(prev_fd[0]);
		close(prev_fd[1]);
	}
	wait_all_execve(all->pipe_num);
	return (0);
}
