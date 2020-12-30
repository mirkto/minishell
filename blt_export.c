/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 20:13:25 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/30 20:13:29 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		blt_export_search_and_inc(t_param *all)
{
	int		i;

	i = 0;
	while (all->env[i])
	{
		if (all->env[i][0] == '\0')
			break ;
		i++;
		if (all->env[i] == NULL)
			all->env = inc_env(&all->env, "\0");
	}
	blt_export_write(all, i);
	return (0);
}

int		blt_export_with_value(t_param *all)
{
	int		i;
	char	**tmp;

	tmp = ft_split(all->cmd[1], '=');
	i = search_key_env(all, tmp[0]);
	if (i > 0)
		blt_export_write(all, i);
	else
		blt_export_search_and_inc(all);
	free_array(&tmp);
	return (0);
}

int		blt_export_without_value(t_param *all)
{
	if (search_key_env(all, all->cmd[1]))
		return (0);
	blt_export_search_and_inc(all);
	return (0);
}

int		blt_export_check_valid_name(t_param *all)
{
	int		i;

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
	{
		return (put_error("not a valid identifier",
							ft_strjoin("$export: ", all->cmd[1])));
	}
	if (ft_strchr(all->cmd[1], '=') == FALSE)
		blt_export_without_value(all);
	else
		blt_export_with_value(all);
	return (0);
}
