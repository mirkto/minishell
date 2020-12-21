/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 16:43:44 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/21 16:43:55 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**inc_env(char ***env)
{
	char	**tmp;
	int		len;

	len = 0;
	while (env[0][len])
			len++;
	tmp = copy_env(*env, len + 1);
	free_buf(env);
	tmp[len] = ft_strdup("\0");
	tmp[++len] = NULL;
	return (tmp);
}

// ---------------------------builtins-----------------------------------------
int		blt_exit()//t_param *all, char **buf)
{
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
// ---------------------------env-----------------------------------------
int		blt_env(t_param *all, int flag)
{
	if (all->cmd_lst[1])
		error_out("No such file or directory", all->cmd_lst[1]);//need add "env:" first arg
	else if (flag == 1)
	{
		char	*alpha;
		int		j;

		alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
		j = -1;
		while (alpha[++j] != '\0')
		{
			all->i = -1;
			while (all->env[++all->i] && ft_strchr(all->env[all->i], '='))
				if (all->env[all->i][0] == alpha[j])
				{
					write(1, "declare -x ", 11);
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
			if (all->env[all->i] != NULL)
				if (ft_strchr(all->env[all->i], '='))
					ft_putendl(all->env[all->i]);
	}
	return (0);
}
// ---------------------------export-----------------------------------------
int		blt_export_util(t_param *all)
{
	char *tmp;

	free(all->env[all->i]);
	tmp = ft_strjoin(all->cmd_lst[1], all->tmp);
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
		if (ft_strncmp(all->env[all->i], all->cmd_lst[1],
				ft_strlen(all->cmd_lst[1])) == 0)
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
			all->env = inc_env(&all->env);
	}
	return (0);
}

int		blt_export_not_equal(t_param *all)
{
	all->flag = 1;
	all->i = -1;
	while (all->env[++all->i])
	{
		if (ft_strncmp(all->env[all->i], all->cmd_lst[1],
				ft_strlen(all->cmd_lst[1])) == 0)
			all->flag = 0;
	}
	while (all->flag == 1)
	{
		all->i = -1;
		while (all->env[++all->i])
			if (all->env[all->i][0] == '\0')
				all->flag = blt_export_util(all);
		if (all->flag == 1)
			all->env = inc_env(&all->env);
	}
	return (0);
}

int		blt_export(t_param *all)
{
	char *tmp;

	if (!all->cmd_lst[1] || all->cmd_lst[1][0] == '#')
	{
		blt_env(all, 1);
		return (0);
	}
	all->i = 0;
	while (all->cmd_lst[1][all->i] != '=' && all->cmd_lst[1][all->i] != '\0')
	{
		if ((!ft_isalpha(all->cmd_lst[1][all->i]) &&
			 !ft_isdigit(all->cmd_lst[1][all->i]) &&
			 all->cmd_lst[1][all->i] != '_')
			 || (all->i == 0 && all->cmd_lst[1][all->i] != '_' &&
			 !ft_isalpha(all->cmd_lst[1][all->i])))
			return (error_out("not a valid identifier", all->cmd_lst[0]));// add more
		all->i++;
	}
	tmp  = ft_strchr(all->cmd_lst[1], '=');
	if (tmp)
		blt_export_equal(all, &tmp);
	else
		blt_export_not_equal(all);
	return (0);
}
// ---------------------------unset-----------------------------------------
int		blt_unset_check_in(t_param *all)
{
	int	len;

	all->i = -1;
	while (all->env[++all->i])
	{
		len = ft_strlen(all->cmd_lst[1]);
		if (!ft_strncmp(all->env[all->i], all->cmd_lst[1], len))
		{
			if (all->env[all->i][len] == '=' || all->env[all->i][len] == '\0')
			{
				// all->env[all->i][0] = '\0';
				ft_bzero(all->env[all->i], ft_strlen(all->env[all->i]));
				break ;
			}
		}
	}
	return (0);
}

int		blt_unset(t_param *all)
{
	blt_unset_check_in(all);
	if (all->cmd_lst[2] || !ft_isalpha(all->cmd_lst[1][0]))
	{
		all->i = 0;
		if (all->cmd_lst[2] || all->cmd_lst[1][0] == '#' ||
								all->cmd_lst[1][0] == '_')
			all->i++;
		while (all->cmd_lst[++all->i])
			error_out("not a valid identifier", all->cmd_lst[all->i]);
		free(all->tmp);
	}
	return (0);
}
// ---------------------------echo-----------------------------------------
int		blt_echo(t_param *all)
{
	all->i = 0;
	while (all->cmd_lst[++all->i])
	{
		if (all->i > 1)
			write(1, " ", 1);
		ft_putstr(all->cmd_lst[all->i]);
	}
	write(1, "\n", 1);
	return (0);
}
// ---------------------------cd-----------------------------------------
int		blt_cd(t_param *all)
{
	if (chdir(all->cmd_lst[1]) == 0)
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
		error_out("No such file or directory", all->cmd_lst[1]);
	return (0);
}
