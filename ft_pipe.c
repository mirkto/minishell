/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 20:45:26 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/18 20:20:55 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		paste_tmp_and_free(t_param *all)
{
	if (all->cmd_flag == 1)
	{
		all->cmd_flag = 0;
		all->cmd = copy_env(all->cmd_tmp, 0);
		free_array(&all->cmd_tmp);
		// ft_putendl("-tmp-");
		// put_cmd(all);
		// ft_putendl("-tmp-");
	}
	return (0);
}

int		cut_and_save_or_check_end(t_param *all, int *i)
{
	if (all->cmd[*i + 1] == NULL)
	{
		free(all->cmd[*i]);
		all->cmd[*i] = NULL;
		return (1);
	}
	else
	{
		all->cmd_tmp = copy_env(&all->cmd[*i + 1], 0);
		all->i = 0;
		while (all->cmd[*i + all->i])
			free(all->cmd[*i + all->i++]);
		all->cmd[*i] = NULL;
	}
	all->cmd_flag = 1;
	*i = 0;
	return (0);
}

int		check_semicolon_and_pipe(t_param *all)
{
	int	i;

	i = 0;
	// all->flag = 0;
	// ft_putendl("\n-str-");
	// put_cmd(all);
	// ft_putendl("-str-\n");
	while (1)
	{
		paste_tmp_and_free(all);
		if (all->cmd[i] == NULL)
		{
			// ft_putendl("not find '|' or ';'");
			return (executor(all));
		}
		else if (!ft_strcmp(all->cmd[i], "|"))
		{
			// // ft_putendl("find '|'");
			// if (cut_and_save_or_check_end(all, i) == 1)
			// 	break ;
			// pipe_conveyor(all);
			// // all->pipe_flag = 0;
			// return (1);
			// // executor(all);
			// // continue ;
			if (cut_and_save_or_check_end(all, &i) == 0)
			{
				pipe_fd_open(all);
				// all->cmd_flag = 0;
				// pipe_conveyor(all);
				// executor(all);
				// return (0);
				continue ;
			}
			continue ;
		}
		else if (!ft_strcmp(all->cmd[i], ";"))
		{
			if (cut_and_save_or_check_end(all, &i) == 0)
				executor(all);
			continue ;
		}
		i++;
	}
	return (0);
}

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

	tmp = check_tmp_on_end(all);

	pipe_conveyor_two(all, fd);

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

// int		pipe_fd_close(t_param *all)
// {
// 	return (0);
// }
