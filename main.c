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
	all->i = 0;
	while (env[++all->i])
		if (!ft_strncmp(env[all->i], "PATH=", 5))
			all->tmp = ft_strdup(&env[all->i][4]);
	//int len = 1;
	// all->i = -1;
	// while(all->tmp[++all->i])
	// 	if (all->tmp[all->i] == ':')
	// 		len++;
	// all->pathes = NULL;
	// if (!(all->pathes = (char **)malloc(sizeof(char *) * len + 1)))
	// 	error_out("dont work malloc buf", "Error");
	all->pathes = ft_split(all->tmp, ':');
	free(all->tmp);
	return (0);
}

int		copy_env(t_param *all, char **env)
{
	int	len;

	len = 0;
	while (env[len])
		len++;
	if (!(all->env = (char **)malloc(sizeof(char *) * (len + 1))))
		error_out("dont work malloc copy env", "Error");
	all->i = -1;
	while (env[++all->i])
		all->env[all->i] = ft_strdup(env[all->i]);
	all->env[all->i] = NULL;
	return (0);
}

int		inc_env(t_param *all)
{
	char	**tmp;
	int		len;

	len = 0;
	while (all->env[len])
		len++;
	if (!(tmp = (char **)malloc(sizeof(char *) * (len + 1 + 1))))
		error_out("dont work malloc inc env", "Error");
	all->i = 0;
	while (all->env[all->i])
	{
		tmp[all->i] = ft_strdup(all->env[all->i]);
		free(all->env[all->i]);
		all->i++;
	}
	free(all->env);
	tmp[all->i] = ft_strdup("\0");
	tmp[++all->i] = NULL;
	all->env = tmp;
	return (0);
}
// ---------------------------builtins-----------------------------------------
int		blt_exit(t_param *all, char **buf)
{
	free_buf(&all->buf_lst);
	free_buf(&all->pathes);
	free(*buf);
	write(1, "exit\n", 5);
	exit(0);
	return (0);
}

int		blt_pwd(t_param *all)
{
	all->tmp = getcwd(NULL, 0);
	ft_putendl(all->tmp);
	free(all->tmp);
	return (0);
}

int		blt_env(t_param *all)
{
	if (all->buf_lst[1])
		error_out("No such file or directory", all->buf_lst[1]);//need add "env:" 
	else
	{
		all->i = 0;
		while (all->env[all->i])
		{
			if (all->env[all->i][0] != '\0')
				ft_putendl(all->env[all->i]);
			all->i++;
		}
	}
	return (0);
}

int		blt_export(t_param *all)
{
	char *tmp;
	
	tmp  = ft_strchr(all->buf_lst[1], '=');
	if (!tmp)
	{
		error_out("not a valid identifier", all->buf_lst[0]);// add more
		return (0);
	}
	tmp++;
	all->tmp = ft_strdup(tmp);
	*tmp = '\0';
	all->flag = 1;
	all->i = -1;
	while (all->env[++all->i])
	{
		if (!ft_strncmp(all->env[all->i], all->buf_lst[1],
				ft_strlen(all->buf_lst[1])))
		{
			// ft_putendl(all->env[all->i]);
			free(all->env[all->i]);
			tmp = ft_strjoin(all->buf_lst[1], all->tmp);
			all->env[all->i] = ft_strdup(tmp);
			// ft_putendl(all->env[all->i]);
			free(all->tmp);
			free(tmp);
			all->flag = 0;
		}
	}
	while (all->flag == 1)
	{
		all->i = -1;
		while (all->env[++all->i])
		{
			if (all->env[all->i][0] == '\0')
			{
				free(all->env[all->i]);
				tmp = ft_strjoin(all->buf_lst[1], all->tmp);
				all->env[all->i] = ft_strdup(tmp);
				free(all->tmp);
				free(tmp);
				all->flag = 0;
			}
		}
		if (all->flag == 1)
			inc_env(all);
	}
	return (0);
}

int		blt_unset(t_param *all)
{
	char *tmp;  
	
	tmp = ft_strdup(all->buf_lst[1]);
	all->tmp = ft_strjoin(tmp, "=");
	free(tmp);
	all->i = -1;
	while (all->env[++all->i])
	{
		if (!ft_strncmp(all->env[all->i], all->tmp, ft_strlen(all->tmp)))
		{
			all->env[all->i][0] = '\0';
			free(all->tmp);
			break ;
		}
	}
	if (all->buf_lst[2] || !ft_isalpha(all->buf_lst[1][0]))
	{
		all->i = 0;
		if (all->buf_lst[2] ||
				all->buf_lst[1][0] == '#' ||
				all->buf_lst[1][0] == '_')
			all->i++;
		while (all->buf_lst[++all->i])
			error_out("not a valid identifier", all->buf_lst[all->i]);
	}
	return (0);
}

int		blt_echo(t_param *all)
{
	all->i = 0;
	while (all->buf_lst[++all->i])
	{
		if (all->i > 1)
			write(1, " ", 1);
		ft_putstr(all->buf_lst[all->i]);
	}
	write(1, "\n", 1);
	return (0);
}

int		blt_cd(t_param *all)
{
	if (chdir(all->buf_lst[1]) == 0)
	{
		all->tmp = NULL;
		all->i = -1;
		while (all->env[++all->i])
		{
			if (!ft_strncmp(all->env[all->i], "PWD=", 4))
			{
				all->tmp = ft_strdup(&all->env[all->i][4]);
				all->flag = -1;
				while (all->env[++all->flag])
					if (!ft_strncmp(all->env[all->flag], "OLDPWD=", 7))
						ft_strcpy(&all->env[all->flag][7], all->tmp);
				free(all->tmp);
				all->tmp = getcwd(NULL, 0);
				ft_strcpy(&all->env[all->i][4], all->tmp);
				break ;
			}
		}
		free(all->tmp);
	}
	else
		error_out("No such file or directory", all->buf_lst[1]);
	return (0);
}
// ---------------------------execve------------------------------------------
int		ft_execve(t_param *all)
{
	char *tmp;

	all->i = -1;
	while (all->pathes[++all->i])
	{
		tmp = ft_strjoin(all->pathes[all->i], "/");
		all->tmp = ft_strjoin(tmp, all->buf_lst[0]);
		free(tmp);
		all->flag = 0;
		int p = fork();
		if (p)
		{
			wait(&p);
		}
		else 
		{
			execve(all->tmp, all->buf_lst, all->env);
			free(all->tmp);
			exit(0);
		}
		free(all->tmp);
		if (all->flag == -1)
			error_out("command not found", all->buf_lst[0]);
	}	
	return (0);
}
// ---------------------------executor------------------------------------------
int		executor(t_param *all, char **buf)
{
	if (all->buf_len < 1 || all->flag == -1)
	{
		if (all->flag == -1)
			ft_putendl("odd number of flag, multiline commands are forbidden");
		free(*buf);
		return (1);
	}
	else if (!ft_strncmp(all->buf_lst[0], "q", 2) ||
			 !ft_strncmp(all->buf_lst[0], "exit", 5))
		blt_exit(all, buf);
	else if (!ft_strncmp(all->buf_lst[0], "pwd", 4) ||
			 !ft_strncmp(all->buf_lst[0], "PWD", 4))
		blt_pwd(all);
	else if (!ft_strncmp(all->buf_lst[0], "env", 4) ||
			 !ft_strncmp(all->buf_lst[0], "ENV", 4))
		blt_env(all);
	else if (!ft_strncmp(all->buf_lst[0], "export", 7))
		blt_export(all);
	else if (!ft_strncmp(all->buf_lst[0], "unset", 6))
		blt_unset(all);
	else if (!ft_strncmp(all->buf_lst[0], "echo", 5))
		blt_echo(all);
	else if (!ft_strncmp(all->buf_lst[0], "cd", 3))
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
	copy_env(&all, env);
	init_pathes(&all, env);
	while (1)
	{
		write(1, "\033[0;32mminishell-0.1$ \033[0m", 26);
		buf = init_buf(&all);
		// init_pathes(&all, env);
		parser(&all, &buf);
		if (executor(&all, &buf) == 1)
			continue ;
		// --------------print_buf---------------
		// all.i = -1;
		// while (all.buf_lst[++all.i])
		// 	ft_putendl(all.buf_lst[all.i]);
		// --------------------------------------
		free_buf(&all.buf_lst);
		// free_buf(&all.pathes);
		free(buf);
	}
	return (0);
}
