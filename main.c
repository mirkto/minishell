/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 18:24:36 by ngonzo            #+#    #+#             */
/*   Updated: 2020/11/27 18:24:40 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*init_buf(t_param *all)
{
	char	*tmp;

	tmp = NULL;
	all->cmd = NULL;
	if (!(tmp = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1)))
	{
		error_out("dont work malloc buf", "Error");
		return (NULL);
	}
	ft_bzero(tmp, BUFFER_SIZE + 1);
	if (!read(0, tmp, BUFFER_SIZE))
	{
		error_out("dont work read buf", "Error");
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
		return (error_out("No such file or directory", argv[1]));
	all.env = copy_env(env, 0);
	all.pathes = init_pathes(&all, env);
	while (1)
	{
		write(1, "\033[0;32mminishell-0.2$ \033[0m", 26);
		if (!(buf = init_buf(&all)))
			return (-1);
		// --------------parser---------------
		parser(&all, &buf);
		if (all.buf_len < 1 || all.flag == -1)
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
			free_buf(&all.cmd);
		}
		free(buf);
	}
	return (0);
}
