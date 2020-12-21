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

void	free_buf(char ***lst)
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

char	**init_pathes(t_param *all, char **env)
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

char	**copy_env(char **env, int len)
{
	char	**new_env;
	int		i;

	if (len == 0)
	{
		while (env[len])
			len++;
	}
	if (!(new_env= (char **)malloc(sizeof(char *) * (len + 1))))
	{
		error_out("dont work malloc copy env", "Error");
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
