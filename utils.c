/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 17:13:51 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/11 19:32:45 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **lst)
{
	int		i;

	i = 0;
	while (lst[i])
	{
		free(lst[i]);
		lst[i] = NULL;
		i++;
	}
	free(lst);
	lst = NULL;
}

int		put_error(char *str_error, char *arg)
{
	if (arg == NULL)
		ft_putendl(str_error);
	else
	{
		write(1, "minishell: ", 11);
		ft_putstr(arg);
		write(1, ": ", 2);
		ft_putstr(str_error);
		write(1, "\n", 1);
	}
	return (-1);
}

char	**split_pathes(t_param *all, char **env)
{
	char	**tmp;
	int		i;

	tmp = NULL;
	i = -1;
	while (env[++i])
		if (!ft_strncmp(env[i], "PATH=", 5))
		{
			all->tmp = ft_strdup(&env[i][5]);
			tmp = ft_split(all->tmp, ':');
			free(all->tmp);
			break ;
		}
	return (tmp);
}

int		check_options(t_param *all, int arg_n)
{
	if (all->cmd[arg_n])
		if (all->cmd[arg_n][0] == '-' && ft_isalpha(all->cmd[arg_n][1]) == TRUE)
			return (TRUE);
	return (FALSE);
}

int		check_back_slash_n(t_param *all, char *tmp)
{
	int i;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\n')
			return (0);
		i++;
	}
	all->tmp = all->buf;
	free(all->buf);
	return (1);
}
