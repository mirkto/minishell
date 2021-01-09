/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 20:15:21 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/30 20:15:24 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_dollar(t_param *all)
{
	int		word;
	int		c;

	word = 0;
	while (all->cmd[++word])
	{
		c = 0;
		all->i = 0;
		all->tmp = ft_strdup(all->cmd[word]);
		while (all->cmd[word][c])
		{
			while (all->cmd[word][c] == '$')
			{
				c++;
				while (ft_isalpha(all->cmd[word][c]) == TRUE)
					c++;
				if (ft_isdigit(all->cmd[word][c]) == TRUE)
					c++;
			}
			all->tmp[all->i++] = all->cmd[word][c++];
		}
		all->tmp[all->i] = '\0';
		free(all->cmd[word]);
		all->cmd[word] = all->tmp;
	}
}

int		blt_export_write(t_param *all, int i, int arg_n)
{
	free(all->env[i]);
	all->env[i] = ft_strdup(all->cmd[arg_n]);
	return (0);
}

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

void	put_cmd(t_param *all)
{
	all->i = -1;
	while (all->cmd[++all->i])
		ft_putendl(all->cmd[all->i]);
}
