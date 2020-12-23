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

int		blt_exit_and_free(t_param *all, int	nbr, int check_nbr)
{
	if (check_nbr == FALSE)
	{
		put_error("numeric argument required",
						ft_strjoin("$exit: ", all->cmd[1]));
		nbr = 255;
	}
	while (nbr > 255 || nbr < 0)
	{
		if (nbr > 255)
			nbr -= 256;
		if (nbr < 0)
			nbr += 256;
	}
	free_array(&all->env);
	free_array(&all->cmd);
	free_array(&all->pathes);
	exit(nbr);
	return (0);
}

int		blt_exit(t_param *all)
{
	write(1, "exit\n", 5);
	if (all->cmd[1])
	{
		if (all->cmd[1][0] == '-' && ft_isalpha(all->cmd[1][1]) == 1)
			return (put_error("Enter without any options!", NULL));
		if (ft_strisdigit(all->cmd[1]) == 0)
			return (blt_exit_and_free(all, 0, FALSE));
		if (all->cmd[2])
			return (put_error("too many arguments", "exit"));
		return (blt_exit_and_free(all, ft_atoi(all->cmd[1]), TRUE));
	}
	return (blt_exit_and_free(all, errno, TRUE));
}

int		blt_pwd(t_param *all)
{
	if (all->cmd[1])
		if (all->cmd[1][0] == '-')
			return (put_error("Enter without any options!", NULL));
	all->tmp = getcwd(NULL, 0);
	ft_putendl(all->tmp);
	free(all->tmp);
	return (0);
}

// ---------------------------env-----------------------------------------

int		blt_env(t_param *all)
{
	if (all->cmd[1])
		return (put_error("Enter without any options and any arguments!", 0));
	all->i = -1;
	while (all->env[++all->i])
		if (all->env[all->i] != NULL)
			if (ft_strchr(all->env[all->i], '='))
				ft_putendl(all->env[all->i]);
	return (0);
}

// ---------------------------export-----------------------------------------

int		blt_export_print(t_param *all)
{
	char	*alpha;
	int		i;
	int		j;

	alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
	all->flag = 0;
	j = -1;
	while (alpha[++j] != '\0')
	{
		i = -1;
		while (all->env[++i])
			if (all->env[i][0] == alpha[j])
			{
				if (ft_strchr(all->env[i], '='))
				{
					write(1, "declare -x ", 11);
					all->i = -1;
					while (all->env[i][++all->i])
					{
						ft_putchar(all->env[i][all->i]);
						if (all->env[i][all->i] == '=' ||
							all->env[i][all->i + 1] == '\0')
							write(1, "\"", 1);
					}
					write(1, "\n", 1);
				}
				else
				{
					write(1, "declare -x ", 11);
					ft_putendl(all->env[i]);
				}
			}
	}
	return (0);
}

int		blt_export_util(t_param *all)
{
	char *tmp;

	free(all->env[all->i]);
	tmp = ft_strjoin(all->cmd[1], all->tmp);
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
		if (ft_strncmp(all->env[all->i], all->cmd[1],
							ft_strlen(all->cmd[1])) == 0)
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
			all->env = inc_env(&all->env, "\0");
	}
	return (0);
}

int		blt_export_not_equal(t_param *all)
{
	all->flag = 1;
	all->i = -1;
	while (all->env[++all->i])
	{
		if (ft_strncmp(all->env[all->i], all->cmd[1],
				ft_strlen(all->cmd[1])) == 0)
			all->flag = 0;
	}
	while (all->flag == 1)
	{
		all->i = -1;
		while (all->env[++all->i])
			if (all->env[all->i][0] == '\0')
				all->flag = blt_export_util(all);
		if (all->flag == 1)
			all->env = inc_env(&all->env, "\0");
	}
	return (0);
}

int		blt_export(t_param *all)
{
	char *tmp;

	if (all->cmd[1])
		if (all->cmd[1][0] == '-')
			return (put_error("Enter without any options!", NULL));
	if (!all->cmd[1] || all->cmd[1][0] == '#')
		return (blt_export_print(all));
	all->i = 0;
	while (all->cmd[1][all->i] != '=' && all->cmd[1][all->i] != '\0')
	{
		if ((!ft_isalpha(all->cmd[1][all->i]) &&
			!ft_isdigit(all->cmd[1][all->i]) &&
			all->cmd[1][all->i] != '_')
			|| (all->i == 0 && all->cmd[1][all->i] != '_' &&
			!ft_isalpha(all->cmd[1][all->i])))
		{
			return (put_error("not a valid identifier",
								ft_strjoin("$export: ", all->cmd[1])));
		}
		all->i++;
	}
	tmp = ft_strchr(all->cmd[1], '=');
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
		len = ft_strlen(all->cmd[1]);
		if (!ft_strncmp(all->env[all->i], all->cmd[1], len))
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
	if (all->cmd[1][0] == '-')
		return (put_error("Enter without any options!", NULL));
	if (all->cmd[2] || (!ft_isalpha(all->cmd[1][0] && all->cmd[1][0] != '_')))
	{
		all->i = 0;
		if (all->cmd[2] || all->cmd[1][0] == '#' ||
								all->cmd[1][0] == '_')
			all->i++;
		while (all->cmd[++all->i])
			put_error("not a valid identifier", all->cmd[all->i]);
		free(all->tmp);
	}
	return (0);
}

// ---------------------------echo-----------------------------------------

int		blt_echo(t_param *all)
{
	int		nbr_arg;
	int		option_n;

	nbr_arg = 1;
	option_n = FALSE;
	if (all->cmd[1][0] == '-' && all->cmd[1][1] == 'n')
	{
		option_n = TRUE;
		nbr_arg++;
	}
	while (all->cmd[nbr_arg])
	{
		ft_putstr(all->cmd[nbr_arg]);
		if (all->cmd[nbr_arg + 1] != '\0')
			write(1, " ", 1);
		nbr_arg++;
	}
	if (option_n == FALSE)
		write(1, "\n", 1);
	return (0);
}

// ---------------------------cd-----------------------------------------

int		blt_cd_replace(t_param *all, char *str)
{
	int		i;
	int		len;

	len = ft_strlen(str);
	i = -1;
	while (all->env[++i])
		if (!ft_strncmp(all->env[i], str, len))
		{
			if (all->env[all->i][len] == '=' || all->env[all->i][len] == '\0')
			{
				free(all->env[i]);
				all->env[i] = ft_strjoin(str, all->tmp);
			}
		}
	return (0);
}

int		blt_cd(t_param *all)
{
	all->tmp = getcwd(NULL, 0);
	if (chdir(all->cmd[1]) == 0)
	{
		blt_cd_replace(all, "OLDPWD");
		free(all->tmp);
		all->tmp = getcwd(NULL, 0);
		blt_cd_replace(all, "PWD");
	}
	else
		put_error("No such file or directory", all->cmd[1]);
	free(all->tmp);
	return (0);
}
