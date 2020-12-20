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

#define BUFFER_SIZE 1024

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

int		error_out(char *str_err, char *arg)
{
	write(1, "minishell: ", 11);
	ft_putstr(arg);
	write(1, ": ", 2);
	ft_putstr(str_err);
	write(1, " | ", 3);
	ft_putstr(strerror(errno));
	write(1, "\n", 1);
	return (-1);
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
	all->i = -1;
	while (env[++all->i])
		if (!ft_strncmp(env[all->i], "PATH=", 5))
			all->tmp = ft_strdup(&env[all->i][5]);
	all->pathes = NULL;
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
int		blt_exit()//t_param *all, char **buf)
{
	// free_buf(&all->buf_lst);
	// free_buf(&all->pathes);
	// free(*buf);
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

int		blt_env(t_param *all, int flag)
{
	if (all->buf_lst[1])
		error_out("No such file or directory", all->buf_lst[1]);//need add "env:" first arg
	else if (flag == 1)
	{
		char	*alpha;
		int		j;

		alpha = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ_";
		j = -1;
		while (alpha[++j] != '\0')
		{
			all->i = -1;
			while (all->env[++all->i] && ft_strchr(all->env[all->i], '='))
				if (all->env[all->i][0] == alpha[j])
				{
					ft_putstr("declare -x ");
					all->flag = -1;
					while (all->env[all->i][++all->flag])
					{
						ft_putchar(all->env[all->i][all->flag]);
						if (all->env[all->i][all->flag] == '=' ||
							all->env[all->i][all->flag + 1] == '\0')
							write(1, "\"", 1);
					}
					write(1, "\n", 1);
				}
		}
		j = -1;
		while (alpha[++j] != '\0')
		{
			all->i = -1;
			while (all->env[++all->i] && !ft_strchr(all->env[all->i], '='))
				if (all->env[all->i][0] == alpha[j])
				{
					ft_putstr("declare -x ");
					ft_putendl(all->env[all->i]);
				}
		}
	}
	else
	{
		all->i = -1;
		while (all->env[++all->i])
			if (all->env[all->i] && ft_strchr(all->env[all->i], '='))
				ft_putendl(all->env[all->i]);
	}
	return (0);
}

int		blt_export_util(t_param *all)
{
	char *tmp;

	free(all->env[all->i]);
	tmp = ft_strjoin(all->buf_lst[1], all->tmp);
	all->env[all->i] = ft_strdup(tmp);
	free(tmp);
	return (0);
}

int		blt_export_equal(t_param *all, char **tmp)
{
	all->tmp = ft_strdup(++*tmp);
	**tmp = '\0';
	all->flag = 1;
	all->i = -1;
	while (all->env[++all->i])
		if (ft_strncmp(all->env[all->i], all->buf_lst[1],
				ft_strlen(all->buf_lst[1])) == 0)
			all->flag = blt_export_util(all);
	while (all->flag == 1)
	{
		all->i = -1;
		while (all->env[++all->i])
			if (all->env[all->i][0] == '\0')
			{
				all->flag = 0;
				blt_export_util(all);
				free(all->tmp);
			}
		if (all->flag == 1)
			inc_env(all);
	}
	return (0);
}

int		blt_export_not_equal(t_param *all)
{
	all->flag = 1;
	all->i = -1;
	while (all->env[++all->i])
	{
		if (ft_strncmp(all->env[all->i], all->buf_lst[1],
				ft_strlen(all->buf_lst[1])) == 0)
			all->flag = 0;
	}
	while (all->flag == 1)
	{
		all->i = -1;
		while (all->env[++all->i])
			if (all->env[all->i][0] == '\0')
				all->flag = blt_export_util(all);
		if (all->flag == 1)
			inc_env(all);
	}
	return (0);
}

int		blt_export(t_param *all)
{
	char *tmp;

	if (!all->buf_lst[1] || all->buf_lst[1][0] == '#')
	{
		blt_env(all, 1);
		return (0);
	}
	all->i = 0;
	while (all->buf_lst[1][all->i] != '=' && all->buf_lst[1][all->i] != '\0')
	{
		if ((!ft_isalpha(all->buf_lst[1][all->i]) &&
			 !ft_isdigit(all->buf_lst[1][all->i]) &&
			 all->buf_lst[1][all->i] != '_')
			 || (all->i == 0 && all->buf_lst[1][all->i] != '_' &&
			 !ft_isalpha(all->buf_lst[1][all->i])))
			return (error_out("not a valid identifier", all->buf_lst[0]));// add more
		all->i++;
	}
	tmp  = ft_strchr(all->buf_lst[1], '=');
	if (tmp)
		blt_export_equal(all, &tmp);
	else
		blt_export_not_equal(all);
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
		if (all->buf_lst[2] || all->buf_lst[1][0] == '#' ||
								all->buf_lst[1][0] == '_')
			all->i++;
		while (all->buf_lst[++all->i])
			error_out("not a valid identifier", all->buf_lst[all->i]);
		free(all->tmp);
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
int		exec_fork(t_param *all)
{
		int p;

		p = fork();
		if (p == 0)
		{
			if (execve(all->tmp, all->buf_lst, all->env) == -1)
				exit(1);
		}
		wait(&p);
	return (0);
}

int		ft_check_dir(char *tmp, char *cmd)
{
	DIR *dir;
	struct dirent *read;
	
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
	return (-1);
}

int		exec_for(t_param *all)
{
	all->tmp = ft_strdup(all->buf_lst[0]);
	exec_fork(all);
	free(all->tmp);
	return (0);
}

int		exec_for_pathes(t_param *all)
{
	char *tmp;

	free_buf(&all->pathes);
	init_pathes(all, all->env);
	all->i = -1;
	while (all->pathes[++all->i])
	{
		tmp = ft_strjoin(all->pathes[all->i], "/");
		if (ft_check_dir(tmp, all->buf_lst[0]) == 0)
		{
			all->tmp = ft_strjoin(tmp, all->buf_lst[0]);
			exec_fork(all);
			free(all->tmp);
			free(tmp);
			break ;
		}
		free(tmp);
	}
	return (0);
}

int		ft_execve(t_param *all)
{
	errno = 0;
	if (all->buf_lst[0][0] == '/' || all->buf_lst[0][0] == '.')
		exec_for(all);
	else
		exec_for_pathes(all);
	if (errno != 0)
		error_out("command not found", all->buf_lst[0]);
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
	else if (!ft_strcmp(all->buf_lst[0], "q") ||	//!ft_strncmp(all->buf_lst[0], "q", 2) ||
			 !ft_strcmp(all->buf_lst[0], "exit"))	//!ft_strncmp(all->buf_lst[0], "exit", 5))
		blt_exit();//all, buf);
	else if (!ft_strcmp(all->buf_lst[0], "pwd") ||	//!ft_strncmp(all->buf_lst[0], "pwd", 4) ||
			 !ft_strcmp(all->buf_lst[0], "PWD"))	//!ft_strncmp(all->buf_lst[0], "PWD", 4))
		blt_pwd(all);
	else if (!ft_strcmp(all->buf_lst[0], "env") ||	//!ft_strncmp(all->buf_lst[0], "env", 4) ||
			 !ft_strcmp(all->buf_lst[0], "ENV"))	//!ft_strncmp(all->buf_lst[0], "ENV", 4))
		blt_env(all, 0);
	else if (!ft_strcmp(all->buf_lst[0], "export"))	//!ft_strncmp(all->buf_lst[0], "export", 7))
		blt_export(all);
	else if (!ft_strcmp(all->buf_lst[0], "unset"))	//!ft_strncmp(all->buf_lst[0], "unset", 6))
		blt_unset(all);
	else if (!ft_strcmp(all->buf_lst[0], "echo"))	//!ft_strncmp(all->buf_lst[0], "echo", 5))
		blt_echo(all);
	else if (!ft_strcmp(all->buf_lst[0], "cd"))		//!ft_strncmp(all->buf_lst[0], "cd", 3))
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
		write(1, "\033[0;32mminishell-0.2$ \033[0m", 26);
		buf = init_buf(&all);
		parser(&all, &buf);
		if (executor(&all, &buf) == 1)
			continue ;
		// --------------print_buf---------------
		// all.i = -1;
		// while (all.buf_lst[++all.i])
		// 	ft_putendl(all.buf_lst[all.i]);
		// --------------------------------------
		free_buf(&all.buf_lst);
		free(buf);
	}
	return (0);
}
