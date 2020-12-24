/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 17:13:51 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/21 17:13:53 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char ***lst)
{
	int		i;

	i = 0;
	while (lst[0][i])
	{
		free(lst[0][i]);
		i++;
	}
	free(*lst);
}

int		put_error(char *str_err, char *arg)
{
	if (arg == NULL)
		ft_putendl(str_err);
	else
	{
		write(1, "minishell: ", 11);
		if (arg[0] == '$')
		{
			ft_putstr(&arg[1]);
			free(arg);
		}
		else
			ft_putstr(arg);
		write(1, ": ", 2);
		ft_putstr(str_err);
		// -------errno-----------
		write(1, " | ", 3);
		ft_putstr(strerror(errno));
		// -----------------------
		write(1, "\n", 1);
	}
	return (-1);
}

char	**split_pathes(t_param *all, char **env)
{
	char	**tmp;
	int		i;

	i = -1;
	while (env[++i])
		if (!ft_strncmp(env[i], "PATH=", 5))
			all->tmp = ft_strdup(&env[i][5]);
	tmp = ft_split(all->tmp, ':');
	free(all->tmp);
	return (tmp);
}

char	**inc_env(char ***env, char *str)
{
	char	**tmp;
	int		len;

	len = 0;
	while (env[0][len])
		len++;
	tmp = copy_env(*env, len + 1);
	free_array(env);
	tmp[len] = ft_strdup(str);
	tmp[++len] = NULL;
	return (tmp);
}

char	**copy_env(char **env, int len)
{
	char	**new_env;
	int		i;

	if (len == 0)
	{
		while (env[len])
			len++;
	}
	if (!(new_env = (char **)malloc(sizeof(char *) * (len + 1))))
	{
		put_error("dont work malloc copy env", "Error");
		return (NULL);
	}
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int		search_in_env(t_param *all, char *str)
{
	int		i;
	int		len;

	len = ft_strlen(str);
	i = 0;
	while (all->env[i])
	{
		if (ft_strncmp(all->env[i], str, len) == 0)
		{
			if (all->env[i][len] == '=' || all->env[i][len] == '\0')
				return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

char	*get_from_env(t_param *all, char *str)
{
	char	*tmp;
	int		i;
	int		len;

	tmp = NULL;
	len = ft_strlen(str);
	i = 0;
	while (all->env[i])
	{
		if (ft_strncmp(all->env[i], str, len) == 0)
		{
			if (all->env[i][len] == '=')
				tmp = ft_strdup(&all->env[i][++len]);
		}
		i++;
	}
	return (tmp);
}