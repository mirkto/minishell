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
		exit (1);
	}
	return (0);
}

int		pipe_conveyor_binder(t_param *all, int fd[2])
{
	if (fork() == 0)
	{
		// ft_putendl("-binder-");
		// int i = -1;
		// while (all->cmd[++i] != NULL)
		// 	ft_putendl(all->cmd[i]);
		if (all->pipe_flag % 2 == 1)
		{
			close(fd[0]);
			dup2(fd[1], 1);
			close(fd[1]);
		}
		if (all->pipe_flag % 2 == 0)	
		{
			close(fd[1]);
			dup2(fd[0], 0);
			close(fd[0]);
		}
		
		// fd_processor(all, 0);
		// ft_execve(all);
		executor(all);
		exit (1);
	}
	return (0);
}

char	**check_tmp_on_end(t_param *all)
{
	int		i;
	char	**tmp;
	
	i = 0;
	tmp = NULL;
	while (1)
	{
		if (all->cmd_tmp[i] == NULL)
		{
			all->cmd_flag = 0;
			free_array(&all->cmd);
			all->cmd = all->cmd_tmp;
			break ;
		}
		if (!ft_strcmp(all->cmd_tmp[i], ";"))
		{
			tmp = copy_env(&all->cmd_tmp[i + 1], 0);
			all->i = 0;
			while (all->cmd_tmp[i + all->i])
				free(all->cmd_tmp[i + all->i++]);
			all->cmd_tmp[i] = NULL;
			free_array(&all->cmd);
			all->cmd = all->cmd_tmp;
			break ;
		}
		if (!ft_strcmp(all->cmd_tmp[i], "|"))
		{
			tmp = copy_env(&all->cmd_tmp[i + 1], 0);
			all->i = 0;
			while (all->cmd_tmp[i + all->i])
				free(all->cmd_tmp[i + all->i++]);
			all->cmd_tmp[i] = NULL;
			free_array(&all->cmd);
			all->cmd = all->cmd_tmp;
			// all->pipe_flag = 1;//----
			break ;
		}
		i++;
	}
	return (tmp);
}

int		pipe_fd_open(t_param *all)
{
	int		fd[2];
	int		fd_tmp[2];
	char	**tmp;
	
	tmp = NULL;
	fd[0] = -2;
	fd[1] = -2;
	fd_tmp[0] = -2;
	fd_tmp[1] = -2;
	pipe(fd);

	// ft_putendl("\n-tmp-");
	// i = -1;
	// while (all->cmd_tmp[++i] != NULL)
	// 	ft_putendl(all->cmd_tmp[i]);
	// ft_putendl("-tmp-\n");

	pipe_conveyor_one(all, fd);
	// pipe_conveyor_binder(all, fd);

	tmp = check_tmp_on_end(all);

	// while (all->pipe_flag == 1)
	// {
	// 	all->pipe_flag = 0;

	// 	pipe_conveyor_binder(all, fd, fd_tmp);
	// 	// pipe_conveyor_two(all, fd);
	// 	// if (tmp != NULL)
	// 	// {
	// 	// 	free_array(&all->cmd_tmp);
	// 	// 	all->cmd_tmp = copy_env(tmp, 0);
	// 	// 	free_array(&tmp);
	// 	// }
	// 	tmp = check_tmp_on_end(all);
	// }

	pipe_conveyor_two(all, fd);
	// pipe_conveyor_binder(all, fd);
	if (tmp != NULL)
	{
		free_array(&all->cmd_tmp);
		all->cmd_tmp = tmp;
	}

	close(fd[0]);
	close(fd[1]);
	wait(NULL);
	wait(NULL);

	// i = 0;
	// while (1)
	// {
	// 	if (all->cmd_tmp[i] != NULL)
	// 	{
	// 		all->cmd_flag = 0;
	// 		// free_array(&all->cmd);
	// 		all->cmd = copy_env(all->cmd_tmp, 0);
	// 		free_array(&all->cmd_tmp);
	// 		break ;
	// 	}
	// 	if (!ft_strcmp(all->cmd_tmp[i], "|"))
	// 	{
	// 		// free_array(&all->cmd);
	// 		// all->cmd = copy_env(all->cmd_tmp, 0);
	// 		all->cmd = (char**)malloc(sizeof(char *) + i);
	// 		all->i = -1;
	// 		while (!ft_strcmp(all->cmd_tmp[++all->i], "|"))
	// 			all->cmd[all->i] = all->cmd_tmp[all->i];
	// 		all->cmd[i] = NULL;
	// 		break ;
	// 	}
	// 	i++;
	// }
	
	// if (all->cmd[*i + 1] == NULL)
	// {
	// 	free(all->cmd[*i]);
	// 	all->cmd[*i] = NULL;
	// 	return (1);
	// }
	// else
	// {
	// 	all->cmd_tmp = copy_env(&all->cmd[*i + 1], 0);
	// 	all->i = 0;
	// 	while (all->cmd[*i + all->i])
	// 		free(all->cmd[*i + all->i++]);
	// 	all->cmd[*i] = NULL;
	// }
	// all->cmd_flag = 1;
	// *i = 0;

	// close(fd[0]);
	// close(fd[1]);
	// wait(NULL);
	// wait(NULL);

	// fd_check_and_close(all);
	// all->redirect = 0;
	return (0);
}
