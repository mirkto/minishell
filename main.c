/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 18:24:36 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/21 17:39:56 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*inits_buf(t_param *all)
{
	char	*tmp;

	tmp = NULL;
	inits_on_start_loop(all);
	if (!(tmp = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1)))
	{
		put_error("dont work malloc buf", "Error");
		return (NULL);
	}
	ft_bzero(tmp, BUFFER_SIZE + 1);
	write(1, "  \b\b", 4);
	return (tmp);
}

char	*inits_buf_and_get_line(t_param *all, char *buf)
{
	char	*tmp;

	if (!(tmp = inits_buf(all)))
		return (NULL);
	write(1, "\033[0;32mminishell-0.4$ \033[0m", 26);
	if (!read(0, tmp, BUFFER_SIZE))
	{
		if (buf != NULL)
		{
			free(tmp);
			return (ft_strdup(buf));
		}
		if (tmp[0] == 0)
			write(1, "exit\n", 5);
		else
			put_error("dont work read buf", "Error");
		return (NULL);
	}
	if (buf != NULL)
	{
		all->tmp = tmp;
		free(tmp);
		tmp = ft_strjoin(buf, all->tmp);
	}
	return (tmp);
}

int		executor(t_param *all)
{
	if (*all->cmd == NULL)
		return (-1);
	fd_processor(all, 0);
	fd_check_and_dup(all);
	if (!ft_strcmp(all->cmd[0], "q") || !ft_strcmp(all->cmd[0], "exit"))
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
	fd_check_and_close(all);
	return (0);
}

void	start_exec_loop(t_param *all)
{
	split_by_semicolon(all);
	check_and_replace_dollar_q(all->cmd);
	check_pipes(all);
}

int		main(int argc, char **argv, char **env)
{
	t_param	all;

	if (argc > 1)
		return (put_error("No such file or directory", argv[1]));
	init_env_and_pathes(&all, env);
	while (1)
	{
		if (!(all.buf = inits_buf_and_get_line(&all, NULL)))
			return (-1);
		while (check_back_slash_n(&all, all.buf) == 1)
			all.buf = inits_buf_and_get_line(&all, all.tmp);
		if (parser(&all, &all.buf) == -1 || check_semicolon(&all) == -1)
			continue ;
		while (all.semicolon_num >= 0)
		{
			start_exec_loop(&all);
			if (all.pipe_num != -1)
				pipe_conveyor(&all);
			else
				executor(&all);
		}
		free_array(all.cmd);
	}
	return (0);
}
