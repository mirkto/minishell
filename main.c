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

int		init_env_and_pathes(t_param *all, char **env)
{
	all->env = copy_env(env, 0);
	if (search_key_env(all, "OLDPWD") == -1)
		all->env = inc_env(all->env, "OLDPWD");
	all->pathes = split_pathes(all, env);
	all->tmp_exit_code = 0;
	all->fd_0 = -2;
	all->fd_1 = -2;
	all->save_fd_0 = -2;
	all->save_fd_1 = -2;
	all->pipe_num = 0;
	return (0);
}

char	*inits_buf(t_param *all)
{
	char	*tmp;

	all->i = 0;
	all->tmp = NULL;
	all->flag = 0;
	all->buf_len = 0;
	all->cmd = NULL;
	all->num_of_toks = 0;
	all->vasya = NULL;
	all->tok = NULL;
	g_exit_code = all->tmp_exit_code;
	all->tmp_exit_code = 0;
	tmp = NULL;
	signal(SIGINT, handler_int_c);
	signal(SIGQUIT, handler_quit_);
	all->redirect = 0;
	all->cmd_tmp = NULL;
	all->semicolon_num = 0;
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
	if (!read(0, tmp, BUFFER_SIZE))
	{
		if (buf != NULL)
		{
			free(tmp);
			tmp = ft_strdup(buf);
			return (tmp);
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
	fd_processor(all);
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
	// free_array(all->cmd);
	return (0);
}

int		main(int argc, char **argv, char **env)
{
	t_param	all;

	if (argc > 1)
		return (put_error("No such file or directory", argv[1]));
	init_env_and_pathes(&all, env);
	while (1)
	{
		write(1, "\033[0;32mminishell-0.4$ \033[0m", 26);
		if (!(all.buf = inits_buf_and_get_line(&all, NULL)))
			return (-1);
		while (check_back_slash_n(&all, all.buf) == 1)
			all.buf = inits_buf_and_get_line(&all, all.tmp);
		if (parser(&all, &all.buf) == -1 || check_semicolon(&all) == -1)
			continue ;

		ft_putendl("/str\\");
		put_cmd(&all); // print cmd
		ft_putendl("\\___/");

		while(all.semicolon_num >= 0)
		{
			split_by_semicolon(&all);
			// ---print_work---
			ft_putstr("\n--");
			ft_putnbr(all.semicolon_num);
			all.semicolon_num--;
			ft_putstr("--\n");
			ft_putendl("/cmd\\");
			put_cmd(&all);
			ft_putendl("\\___/");
			ft_putendl("/tmp\\");
			int	c = -1;
			while (all.cmd_tmp[++c] != NULL)
				ft_putendl(all.cmd_tmp[c]);
			if (all.cmd_tmp[0] == NULL)
				ft_putendl("#free");
			ft_putendl("\\___/");
			// ---print_work---
			executor(&all);
		}
		free_array(all.cmd);
	}
	return (0);
}
