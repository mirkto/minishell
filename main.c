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

#define BUFFER_SIZE 99


void	free_buf(char ***lst)
{
	int	i;

	i = 0;
	while (lst[0][i])
	{
		free(lst[0][i]);
		i++;
	}
	free(*lst);
	// ft_putendl("---");
}

int		error_out(char *str, char *print)
{
	write(1, "minishell: ", 11);
	ft_putstr(print);
	write(1, ": ", 2);
	ft_putstr(str);
	write(1, "\n", 1);
	return (0);
}

char	*init_buf(t_param *all)
{
	char	*tmp;

	tmp = NULL;
	all->buf_lst = NULL;
	if (!(tmp = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1)))
		error_out("dont work malloc buf", "Error");
	ft_bzero(tmp, BUFFER_SIZE + 1);
	if (!read(0, tmp, BUFFER_SIZE))
		error_out("dont work read buf", "Error");
	return (tmp);
}

int		init_pathes(t_param *all, char **env)
{
	all->pathes = NULL;
	all->i = -1;
	while (env[++all->i])
		if (!ft_strncmp(env[all->i], "PATH=", 5))
			all->tmp = ft_strdup(&env[all->i][4]);
	all->pathes = ft_split(all->tmp, ':');
	free(all->tmp);
	// all->i = -1;
	// while (all->pathes[++all->i])
	// {
	// 	all->tmp = ft_strjoin(all->pathes[all->i], "/");
	// 	// free(all->pathes[all->i]);
	// 	all->pathes[all->i] = all->tmp;
	// }
	// free(all->tmp);
	return (0);
}

int		copy_env(t_param *all, char **env)
{
	int	len;

	len = 0;
	while (env[len])
		len++;
	// ft_putnbr(len);
	// ft_putendl("");
	all->env = (char **)malloc(sizeof(char *) * (len + 1));
	// all->env[len][0] = '\0';
	all->i = 0;
	while (env[all->i])
	{
		all->env[all->i] = ft_strdup(env[all->i]);
		all->i++;
	}
	all->env[all->i] = NULL;
	return (0);
}
// ---------------------------------------------------------
int		implement(t_param *all, char **buf, char ***env)
{
	if (all->buf_len < 1 || all->flag == -1)
	{
		if (all->flag == -1)
			ft_putendl("odd number of flag, multiline commands are forbidden");
		free_buf(&all->pathes);
		free(*buf);
		return (1);
	}
	else if (!ft_strncmp(all->buf_lst[0], "q", 2) ||
			 !ft_strncmp(all->buf_lst[0], "exit", 5))
	{
		write(1, "exit\n", 5);
		exit(0);
	}
	else if (!ft_strncmp(all->buf_lst[0], "pwd", 4) ||
			 !ft_strncmp(all->buf_lst[0], "PWD", 4))
	{
		all->tmp = getcwd(NULL, 0);
		ft_putendl(all->tmp);
		free(all->tmp);
	}
	else if (!ft_strncmp(all->buf_lst[0], "env", 4) ||
			 !ft_strncmp(all->buf_lst[0], "ENV", 4))
	{
		if (all->buf_lst[1])
			error_out("No such file or directory", all->buf_lst[1]);//need add "env:" 
		else
		{
			all->i = 0;
			// while (env[0][++all->i])
			// 	if (env[0][all->i][0] != '\0')
			// 		ft_putendl(env[0][all->i]);
			while (all->env[all->i])
			{
				if (all->env[all->i] != '\0')
					ft_putendl(all->env[all->i]);
				all->i++;
			}
		}
	}
	else if (!ft_strncmp(all->buf_lst[0], "export", 7))
	{
		char *s  = ft_strchr(all->buf_lst[1], '=');
		if (!s)
		{
			ft_putendl("-1-");
			return (0);
		}
		s++;
		all->tmp = ft_strdup(s);
		*s = '\0';
		// free(s);
		// char *s  = ft_strdup(all->buf_lst[1]);
		// all->tmp = ft_strjoin(s, "=");
		ft_putendl("-2-");
		all->i = -1;
		while (env[0][++all->i])
		{
			// ft_putstr(all->buf_lst[1]);
			// ft_putnbr(ft_strlen(all->buf_lst[1]));
			// ft_putendl(" -3-");
			// ft_putendl(all->tmp);
			if (!ft_strncmp(env[0][all->i], all->buf_lst[1], ft_strlen(all->buf_lst[1])))
			{
				ft_putendl("---");
				ft_putendl(env[0][all->i]);
				// free(env[0][all->i]);
				s = ft_strjoin(all->buf_lst[1], all->tmp);
				ft_strcpy(env[0][all->i], s);
				ft_putendl(env[0][all->i]);
				free(all->tmp);
				free(s);
				// free(all->buf_lst[1]);
			}
		}

		// if (setenv(all->buf_lst[1], all->buf_lst[2], 0) < 0 || all->buf_lst[3])
		// {
		// 	all->i = 2;
		// 	while (all->buf_lst[++all->i])
		// 		error_out("not a valid identifier", all->buf_lst[all->i]);
		// }
	}
	else if (!ft_strncmp(all->buf_lst[0], "unset", 6))
	{
		char *s  = ft_strdup(all->buf_lst[1]);
		all->tmp = ft_strjoin(s, "=");
		free(s);
		all->flag = 0;
		all->i = -1;
		while (env[0][++all->i])
		{
			if (!ft_strncmp(env[0][all->i], all->tmp, 4))
			{
				// ft_putendl(&env[0][all->i][0]);
				env[0][all->i][0] = '\0';
				all->flag = 1;
				free(all->tmp);
				break ;
			}
		}
		if (all->flag == 1 || all->buf_lst[2])
		{
			all->i = 1;
			while (all->buf_lst[++all->i])
				error_out("not a valid identifier", all->buf_lst[all->i]);
		}
	}
	else if (!ft_strncmp(all->buf_lst[0], "echo", 5))
	{
		all->i = 0;
		while (all->buf_lst[++all->i])
		{
			if (all->i > 1)
				write(1, " ", 1);
			ft_putstr(all->buf_lst[all->i]);
		}
		write(1, "\n", 1);
	}
	else if (!ft_strncmp(all->buf_lst[0], "cd", 3))
	{
		if (chdir(all->buf_lst[1]) == 0)
		{
			all->tmp = NULL;
			all->i = -1;
			while (env[0][++all->i])
			{
				if (!ft_strncmp(env[0][all->i], "PWD=", 4))
				{
					all->tmp = ft_strdup(&env[0][all->i][4]);
					all->flag = -1;
					while (env[0][++all->flag])
						if (!ft_strncmp(env[0][all->flag], "OLDPWD=", 7))
							ft_strcpy(&env[0][all->flag][7], all->tmp);
					free(all->tmp);
					all->tmp = getcwd(NULL, 0);
					ft_strcpy(&env[0][all->i][4], all->tmp);
					break ;
				}
			}
			free(all->tmp);
		}
		else
			error_out("No such file or directory", all->buf_lst[1]);
	}
	else
	{
		all->i = -1;
		while (all->pathes[++all->i])
		{
			char *s = ft_strjoin(all->pathes[all->i], "/");
			all->tmp = ft_strjoin(s, all->buf_lst[0]);
			free(s);
			// ft_putendl(s);
			int p = fork();
			if (p)
			{
				wait(&p);
			}
			else 
			{
				execve(all->tmp, all->buf_lst, *env);
				exit(0);
			}
			free(all->tmp);
		}			
		error_out("command not found", all->buf_lst[0]);
	}
	return (0);
}
// ---------------------------------------------------------
int		main(int argc, char **argv, char **env)
{
	char	*buf;
	t_param	all;

	if (argc > 1)
		return (error_out("No such file or directory", argv[1]));
	copy_env(&all, env);
	while (1)
	{
		write(1, "\033[0;32mminishell-0.1$ \033[0m", 26);
		buf = init_buf(&all);
		init_pathes(&all, env);
		parser(&all, &buf);
		if (implement(&all, &buf, &env) == 1)
			continue ;
		// --------------print_buf---------------
		// all.i = 0;
		// while (all.buf_lst[all.i])
		// {
		// 	ft_putendl(all.buf_lst[all.i]);
		// 	all.i++;
		// }
		// --------------------------------------
		free_buf(&all.buf_lst);
		free_buf(&all.pathes);
		free(buf);
	}
	return (0);
}
