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

int		check_pipes_and_end(t_param *all)
{
	int	i;

	i = 0;
	while (1)
	{
		if (all->cmd_flag == 1)
		{
			all->cmd_flag = 0;
			all->cmd = copy_env(all->cmd_tmp, 0);
			free_array(&all->cmd_tmp);
			// put_cmd(all);
		}
		if (all->cmd[i] == NULL)
		{
			// ft_putendl("not find '|' or ';'");
			break ;
		}
		else if (!ft_strcmp(all->cmd[i], "|"))
		{
			ft_putendl("find '|'");
			if (all->cmd[i + 1] == NULL)
			{
				free(all->cmd[i]);
				all->cmd[i] = NULL;
				break ;
			}
			else 
			{
				all->cmd_tmp = copy_env(&all->cmd[i + 1], 0);
				all->i = 0;
				while (all->cmd[i + all->i])
					free(all->cmd[i + all->i++]);
				all->cmd[i] = NULL;
			}
			pipe_conveyor(all);
			all->cmd_flag = 1;
			return (1);
		}
		else if (!ft_strcmp(all->cmd[i], ";"))
		{
			// ft_putendl("not find ';'");
			if (all->cmd[i + 1] == NULL)
			{
				free(all->cmd[i]);
				all->cmd[i] = NULL;
				break ;
			}
			else 
			{
				all->cmd_tmp = copy_env(&all->cmd[i + 1], 0);
				all->i = 0;
				while (all->cmd[i + all->i])
					free(all->cmd[i + all->i++]);
				all->cmd[i] = NULL;
			}
			// put_cmd(all);
			all->cmd_flag = 1;
			return (2);
		}
		i++;
	}
	executor(all);
	return (0);
}

int		pipe_conveyor(t_param *all)
{
	int		fd[2];

	fd[0] = all->fd_0;
	fd[1] = all->fd_1;
	pipe(fd);
	if (fork() == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		// all->tmp = ;
		// all->cmd = ;
		executor(all);
		// execlp("ls", "ls", "-lR", NULL);
		// execve(all->tmp, all->cmd, all->env);
	}
	if (fork() == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		// all->tmp = ;
		// all->cmd = ;
		executor(all);
		// execlp("grep", "grep", "^d", NULL);
		// execve(all->tmp, all->cmd, all->env);
	}
	close(fd[0]);
	close(fd[1]);
	wait(NULL);
	wait(NULL);
	return (0);
}