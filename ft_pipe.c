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

int		pipe_conveyor_one(t_param *all, int fd[2])
{
	if (fork() == 0)
	{
		// ft_putendl("-1-");
		// int i = -1;
		// while (all->cmd[++i] != NULL)
		// 	ft_putendl(all->cmd[i]);

		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		fd_processor(all);
		ft_execve(all);
		// executor(all);
		exit (1);
	}
	return (0);
}

int		pipe_conveyor_two(t_param *all, int fd[2])
{
	if (fork() == 0)
	{
		// ft_putendl("-2-");
		// int i = -1;
		// while (all->cmd[++i] != NULL)
		// 	ft_putendl(all->cmd[i]);

		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		fd_processor(all);
		ft_execve(all);
		// executor(all);
		exit (1);
	}
	return (0);
}

int		check_tmp_on_end(t_param *all)
{
	int		i;
	char	**tmp;
	
	i = 0;
	tmp = NULL;
	while (1)
	{
		
		if (all->cmd_tmp[i + 1] == NULL)
		{
			free_array(all->cmd);
			all->cmd = copy_env(all->cmd_tmp, 0);
			// free_array(all->cmd_tmp);
			break ;
		}
		if (!ft_strcmp(all->cmd_tmp[i], ";") || !ft_strcmp(all->cmd_tmp[i], "|"))
		{
			tmp = copy_env(&all->cmd_tmp[i + 1], 0);
			all->i = 0;
			while (all->cmd_tmp[i + all->i])
				free(all->cmd_tmp[i + all->i++]);
			all->cmd_tmp[i] = NULL;
			free_array(all->cmd);
			all->cmd = copy_env(all->cmd_tmp, 0);
			if (tmp != NULL)
			{
				free_array(all->cmd_tmp);
				all->cmd_tmp = copy_env(tmp, 0);
				free_array(tmp);
			}
			// free_array(all->cmd_tmp);
			break ;
		}
		i++;
	}
		return (0);
}

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

int		pipe_fd_open(t_param *all)
{
	int		prev_fd[2];
	int		next_fd[2];
	
	prev_fd[0] = -2;
	prev_fd[1] = -2;
	// ------------
	int i = 0;
	while (i <= all->pipe_num)
	{
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
			// fd_processor(all);
			// ft_execve(all);
			executor(all);
			exit (1);
		}
		// check_tmp_on_end(all);

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
