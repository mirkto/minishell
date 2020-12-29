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
		put_error("numeric argument required",
			ft_strjoin("$exit: ", all->cmd[1]));
	while (nbr > 255)
		nbr -= 256;
	while (nbr < 0)
		nbr += 256;
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
		if (all->cmd[2])
			return (put_error("too many arguments", "exit"));
		if (check_options(all) == TRUE)
			return (put_error("Enter without any options!", NULL));
		if (ft_strisdigit(all->cmd[1]) == FALSE)
			return (blt_exit_and_free(all, 255, FALSE));
		return (blt_exit_and_free(all, ft_atoi(all->cmd[1]), TRUE));
	}
	return (blt_exit_and_free(all, errno, TRUE));
}

// ---------------------------pwd-----------------------------------------

int		blt_pwd(t_param *all)
{
	if (check_options(all) == TRUE)
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
		return (put_error("Enter without any options or arguments!", NULL));
	all->i = -1;
	while (all->env[++all->i])
		if (all->env[all->i] != NULL)
			if (ft_strchr(all->env[all->i], '='))
				ft_putendl(all->env[all->i]);
	return (0);
}

// ---------------------------export-----------------------------------------

int		blt_export_put(t_param *all, int index)
{
	int		i;

	if (all->env[index][0] != '\0')
		write(1, "declare -x ", 11);
	if (ft_strchr(all->env[index], '='))
		{
		i = 0;
		while (all->env[index][i])
		{
			ft_putchar(all->env[index][i]);
			if (all->env[index][i] == '=')
				write(1, "\"", 1);
			if (all->env[index][i + 1] == '\0')
				write(1, "\"", 1);
			i++;
		}
		write(1, "\n", 1);
		}
	else if (all->env[index][0] != '\0')
		ft_putendl(all->env[index]);
	return (0);
}

int		blt_export_print(t_param *all)
{
	char	**tmp_env;
	char	*tmp;
	int		i;

	tmp_env = all->env;
	i = 1;
	while (tmp_env[i] != NULL)
	{
		if (ft_strcmp(tmp_env[i - 1], tmp_env[i]) > 0)
		{
			tmp = tmp_env[i - 1];
			tmp_env[i - 1] = tmp_env[i];
			tmp_env[i] = tmp;
			i = 0;
		}
		i++;
	}
	i = -1;
	while (tmp_env[++i])
		blt_export_put(all, i);
	return (0);
}

int		blt_export_with_value(t_param *all)
{
	int	i;
	char **tmp;

	tmp = ft_split(all->cmd[1], '=');
	i = search_key_env(all, tmp[0]);
	if (i > 0)
	{
		free(all->env[i]);
		all->env[i] = ft_strdup(all->cmd[1]);
	}
	else
	{
		i = 0;
		while (all->env[i])
		{
			if (all->env[i][0] == '\0')
				break ;
			i++;
			if (all->env[i] == NULL)
				all->env = inc_env(&all->env, "\0");
		}
		free(all->env[i]);
		all->env[i] = ft_strdup(all->cmd[1]);
	}
	free_array(&tmp);
	return (0);
}

int		blt_export_without_value(t_param *all)
{
	int	i;

	if (search_key_env(all, all->cmd[1]))
		return (0);
	i = 0;
	while (all->env[i])
	{
		if (all->env[i][0] == '\0')
			break ;
		i++;
		if (all->env[i] == NULL)
			all->env = inc_env(&all->env, "\0");
	}
	free(all->env[i]);
	all->env[i] = ft_strdup(all->cmd[1]);
	return (0);
}

int		blt_export_check_valid_name(t_param *all)
{
	int	i;

	i = 0;
	if (all->cmd[1][i] != '_' && ft_isalpha(all->cmd[1][i]) == FALSE)
		return (ERROR);
	i++;
	while (all->cmd[1][i] != '=' && all->cmd[1][i] != '\0')
	{
		if (ft_isalpha(all->cmd[1][i]) == FALSE &&
			ft_isdigit(all->cmd[1][i]) == FALSE &&
			all->cmd[1][i] != '_')
		{
			return (ERROR);
		}
		i++;
	}
	return (0);
}

int		blt_export(t_param *all)
{
	if (check_options(all) == TRUE)
		return (put_error("Enter without any options!", NULL));
	if (!all->cmd[1] || all->cmd[1][0] == '#')
		return (blt_export_print(all));
	if (blt_export_check_valid_name(all) == ERROR)
		return (put_error("not a valid identifier",
								ft_strjoin("$export: ", all->cmd[1])));
	if (ft_strchr(all->cmd[1], '=') == FALSE)
		blt_export_without_value(all);
	else
		blt_export_with_value(all);
	return (0);
}

// ---------------------------unset-----------------------------------------

int		blt_unset_check_in(t_param *all)
{
	all->i = 0;
	if ((all->i = search_key_env(all, all->cmd[1])))
		ft_bzero(all->env[all->i], ft_strlen(all->env[all->i]));
	return (0);
}

int		blt_unset(t_param *all)
{
	if (!all->cmd[1])
		return (0);
	if (check_options(all) == TRUE)
		return (put_error("Enter without any options!", NULL));
	blt_unset_check_in(all);
	if (all->cmd[2] || (!(ft_isalpha(all->cmd[1][0])) && all->cmd[1][0] != '_'))
	{
		all->i = 0;
		if (all->cmd[2] || all->cmd[1][0] == '#' || all->cmd[1][0] == '_')
			all->i++;
		while (all->cmd[++all->i])
			put_error("not a valid identifier", all->cmd[all->i]);
		return (-1);
		free(all->tmp);
	}
	return (0);
}

// ---------------------------echo-----------------------------------------

int		blt_echo(t_param *all)
{
	int		arg;
	int		option_n;

	if (!all->cmd[1])
		return (write(1, "\n", 1) - 1);
	arg = 1;
	option_n = FALSE;
	if (!ft_strcmp(all->cmd[1], "-n"))
	{
		option_n = TRUE;
		arg++;
	}
	else if (check_options(all) == TRUE)
		return (put_error("Enter with option ’-n’ only!", NULL));
	while (all->cmd[arg])
	{
		ft_putstr(all->cmd[arg]);
		if (all->cmd[arg + 1] != '\0')
			write(1, " ", 1);
		arg++;
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
		if (ft_strncmp(all->env[i], str, len) == 0)
		{
			if (all->env[i][len] == '=' || all->env[i][len] == '\0')
			{
				free(all->env[i]);
				all->env[i] = ft_strstrjoin(str, "=", all->tmp);
			}
		}
	return (0);
}

int		blt_cd(t_param *all)
{
	if (!all->cmd[1] || (all->cmd[1][0] == '~' && all->cmd[1][1] == '\0'))
	{
		all->tmp = get_value_env(all, "HOME");
		if (chdir(all->tmp))
			return (put_error("HOME not set", "cd"));
		free(all->tmp);
		return (0);
	}
	if (all->cmd[1][0] == '-' && all->cmd[1][1] == '\0')
		{
			if (!(all->tmp = get_value_env(all, "OLDPWD")))
				return (put_error("OLDPWD not set", "cd"));
			ft_putendl(all->tmp);
			free(all->cmd[1]);
			all->cmd[1] = all->tmp;
		}
	all->tmp = getcwd(NULL, 0);
	if (chdir(all->cmd[1]) == 0)
	{
		blt_cd_replace(all, "OLDPWD");
		free(all->tmp);
		all->tmp = getcwd(NULL, 0);
		blt_cd_replace(all, "PWD");
	}
	else
		return (put_error("No such file or directory", all->cmd[1]));
	free(all->tmp);
	return (0);
}
