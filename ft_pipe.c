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

int		cut_and_save(t_param *all, int i)
{
	if (all->cmd[i + 1] == NULL)
	{
		free(all->cmd[i]);
		all->cmd[i] = NULL;
		return (1);
	}
	else 
	{
		all->cmd_tmp = copy_env(&all->cmd[i + 1], 0);
		all->i = 0;
		while (all->cmd[i + all->i])
			free(all->cmd[i + all->i++]);
		all->cmd[i] = NULL;
	}
	all->cmd_flag = 1;
	return (0);
}

int		check_pipes_and_end(t_param *all)
{
	int	i;

	i = 0;
	// all->flag = 0;
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
			ft_putendl("not find '|' or ';'");
			break ;
		}
		else if (!ft_strcmp(all->cmd[i], "|"))
		{
			// ft_putendl("find '|'");
			if (cut_and_save(all, i) == 1)
				break ;
			pipe_conveyor(all);
			// all->pipe_flag = 0;
			return (1);
		}
		else if (!ft_strcmp(all->cmd[i], ";"))
		{
			// ft_putendl("not find ';'");
			if (cut_and_save(all, i) == 1)
				break ;
			// put_cmd(all);
			return (2);
		}
		i++;
	}
	// if (all->pipe_flag == 1)
	// 	all->pipe_flag = executor(all);
	// else
	executor(all);
	return (0);
}

int		pipe_conveyor(t_param *all)
{
	int		fd[2];

	// all->save_fd_1 = dup(1);
	// all->save_fd_0 = dup(0);	

	fd[0] = all->fd_0;
	fd[1] = all->fd_1;
	
	// ------print--------
	// ft_putendl("-tmp-");
	// ft_putendl(all->tmp);
	// ft_putendl("-cmd-");
	// int i = -1;
	// while (all->cmd[++i] != NULL)
	// 	ft_putendl(all->cmd[i]);
	// ft_putendl("---");

	pipe(fd);
	if (fork() == 0)
	{
		ft_putendl("-1-");
		int i = -1;
		while (all->cmd[++i] != NULL)
			ft_putendl(all->cmd[i]);

		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		// all->tmp = ;
		// all->cmd = ;
		executor(all);
		// execlp("ls", "ls", "-lR", NULL);
		// execve(all->tmp, all->cmd, all->env);
		exit (1);
	}
	if (fork() == 0)
	{
		ft_putendl("-2-");
		int i = -1;
		while (all->cmd[++i] != NULL)
			ft_putendl(all->cmd[i]);

		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		// all->tmp = ;
		// all->cmd = ;
		executor(all);
		// execlp("grep", "grep", "^d", NULL);
		// execve(all->tmp, all->cmd, all->env);
		exit (1);
	}
	close(fd[0]);
	close(fd[1]);
	wait(NULL);
	wait(NULL);

	// dup2(all->save_fd_1, 1);
	// dup2(all->save_fd_0, 0);
	// if (all->save_fd_1 != -2)
	// 	all->save_fd_1 = fd_close(all->save_fd_1);
	// if (all->save_fd_0 != -2)
	// 	all->save_fd_0 = fd_close(all->save_fd_0);

	return (0);
}