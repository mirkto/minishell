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

int		blt_export_search_and_inc(t_param *all, int arg_n)
{
	int		i;

	i = 0;
	while (all->env[i])
	{
		if (all->env[i][0] == '\0')
			break ;
		i++;
		if (all->env[i] == NULL)
			all->env = inc_env(all->env, "\0");
	}
	blt_export_write(all, i, arg_n);
	return (0);
}

int		blt_export_with_value(t_param *all, int arg_n)
{
	int		i;
	char	**tmp;

	tmp = ft_split(all->cmd[arg_n], '=');
	i = search_key_env(all, tmp[0]);
	if (i >= 0)
		blt_export_write(all, i, arg_n);
	else
		blt_export_search_and_inc(all, arg_n);
	free_array(tmp);
	return (0);
}

int		blt_export_without_value(t_param *all, int arg_n)
{
	if (search_key_env(all, all->cmd[arg_n]) == -1)
		return (0);
	blt_export_search_and_inc(all, arg_n);
	return (0);
}

int		blt_export_check_valid_name(t_param *all, int arg_n)
{
	int		i;

	i = 0;
	if (all->cmd[arg_n][i] != '_' && ft_isalpha(all->cmd[arg_n][i]) == FALSE)
		return (ERROR);
	if (all->cmd[arg_n][i] == '=')
		return (ERROR);
	i++;
	while (all->cmd[arg_n][i] != '=' && all->cmd[arg_n][i] != '\0')
	{
		if (ft_isalpha(all->cmd[arg_n][i]) == FALSE &&
			ft_isdigit(all->cmd[arg_n][i]) == FALSE &&
			all->cmd[arg_n][i] != '_')
		{
			return (ERROR);
		}
		i++;
	}
	return (0);
}

int		blt_export(t_param *all)
{
	int		arg_n;

	if (!all->cmd[1] || all->cmd[1][0] == '#')
		return (blt_export_print(all));
	arg_n = 1;
	while (all->cmd[arg_n])
	{
		if (check_options(all, arg_n) == TRUE)
			return (put_error("Enter without any options!", NULL));
		if (blt_export_check_valid_name(all, arg_n) == ERROR)
			put_error("not a valid identifier",
						ft_strjoin("@#&export: ", all->cmd[arg_n]));
		else if (ft_strchr(all->cmd[arg_n], '=') == FALSE)
			blt_export_without_value(all, arg_n);
		else
			blt_export_with_value(all, arg_n);
		arg_n++;
	}
	return (0);
}
