/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 21:31:32 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/21 21:31:35 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exec_fork(t_param *all)
{
	int		status;
	int		wr;

	status = fork();
	if (status == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(all->tmp, all->cmd, all->env) == -1)
		{
			put_error("No such file or directory", all->cmd[0]);
			exit(127);
		}
	}
	wr = wait(&status);
	if (wr != -1)
	{
		all->tmp_exit_code = WEXITSTATUS(status);
		return (1);
	}
	return (0);
}

int		exec_check_dir(char *tmp, char *cmd)
{
	DIR				*dir;
	struct dirent	*read;

	dir = opendir(tmp);
	while (dir)
	{
		errno = 0;
		if ((read = readdir(dir)) != NULL)
		{
			if (ft_strcmp(read->d_name, cmd) == 0)
				return (closedir(dir));
		}
		else
		{
			if (errno == 0)
			{
				closedir(dir);
				return (1);
			}
			closedir(dir);
			return (-1);
		}
	}
	return (-2);
}

int		exec_check_path(t_param *all)
{
	char	*tmp;
	int		i;

	if (search_key_env(all, "PATH") == -1)
		return (put_error("No such file or directory", all->cmd[0]));
	free_array(&all->pathes);
	all->pathes = split_pathes(all, all->env);
	i = -1;
	while (all->pathes[++i])
	{
		tmp = ft_strjoin(all->pathes[i], "/");
		if (exec_check_dir(tmp, all->cmd[0]) == 0)
		{
			all->tmp = ft_strjoin(tmp, all->cmd[0]);
			exec_fork(all);
			free(all->tmp);
			free(tmp);
			return (1);
		}
		free(tmp);
	}
	return (0);
}

int		exec_absolute_and_relative_path(t_param *all)
{
	int	flag;

	flag = 0;
	all->tmp = ft_strdup(all->cmd[0]);
	flag = exec_fork(all);
	free(all->tmp);
	return (flag);
}

int		ft_execve(t_param *all)
{
	int	flag;

	flag = 0;
	if (all->cmd[0][0] == '/' || all->cmd[0][0] == '.')
		flag = exec_absolute_and_relative_path(all);
	else
		flag = exec_check_path(all);
	if (flag == 0)
	{
		all->tmp_exit_code = 127;
		put_error("command not found", all->cmd[0]);
	}
	return (0);
}
