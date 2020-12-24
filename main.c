/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 18:24:36 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/24 23:12:07 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		init_env(t_param *all, char **env)
{
	// char	*tmp;

	all->env = copy_env(env, 0);
	// tmp = get_from_env(all, "OLDPWD");
	// if (tmp)
	// 	all->env = inc_env(&all->env, "OLDPWD");
	// else
		all->pathes = split_pathes(all, env);
	return (0);
}

char	*init_buf_and_get_line(t_param *all)
{
	char	*tmp;

	tmp = NULL;
	all->cmd = NULL;
	if (!(tmp = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1)))
	{
		put_error("dont work malloc buf", "Error");
		return (NULL);
	}
	ft_bzero(tmp, BUFFER_SIZE + 1);
	if (!read(0, tmp, BUFFER_SIZE))
	{
		put_error("dont work read buf", "Error");
		return (NULL);
	}
	return (tmp);
}

// ---------------------------executor-----------------------------------------

int		executor(t_param *all)
{
	if (!ft_strcmp(all->cmd[0], "q") ||
			!ft_strcmp(all->cmd[0], "exit"))
		blt_exit(all);
	else if (!ft_strcmp(all->cmd[0], "pwd") ||
			!ft_strcmp(all->cmd[0], "PWD"))
		blt_pwd(all);
	else if (!ft_strcmp(all->cmd[0], "env") ||
			!ft_strcmp(all->cmd[0], "ENV"))
		blt_env(all);
	else if (!ft_strcmp(all->cmd[0], "export"))
		blt_export(all);
	else if (!ft_strcmp(all->cmd[0], "unset"))
		blt_unset(all);
	else if (!ft_strcmp(all->cmd[0], "echo"))
		blt_echo(all);
	else if (!ft_strcmp(all->cmd[0], "cd"))
		blt_cd(all);
	else
		ft_execve(all);
	return (0);
}

// --------------------------main-------------------------------

int		main(int argc, char **argv, char **env)
{
	char	*buf;
	t_param	all;

	if (argc > 1)
		return (put_error("No such file or directory", argv[1]));
	init_env(&all, env);
	while (1)
	{
		write(1, "\033[0;32mminishell-0.3$ \033[0m", 26);
		if (!(buf = init_buf_and_get_line(&all)))
			return (-1);
		// --------------parser---------------
		// parser(&all, &buf);
		// free(buf);
		if (all.buf_len < 1 || all.flag == -1 || parser(&all, &buf) == -1)
		{
			if (all.flag == -1)
			{
				ft_putendl("odd number of commas");
				ft_putendl("multiline commands are forbidden");
			}
		}
		// -----------------------------------
		else
		{
			executor(&all);
			// --------------print_buf---------------
			// all.i = -1;
			// while (all.cmd[++all.i])
			// 	ft_putendl(all.cmd[all.i]);
			// --------------------------------------
			free_array(&all.cmd);
		}
		free(buf);
	}
	return (0);
}
