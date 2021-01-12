/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 16:43:44 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/11 19:25:16 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		blt_exit_and_free(t_param *all, int nbr, int check_nbr)
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
		{
			all->tmp_exit_code = 1;
			return (put_error("too many arguments", "exit"));
		}
		if (check_options(all, 1) == TRUE)
			return (put_error("Enter without any options!", NULL));
		if (ft_strisdigit(all->cmd[1]) == FALSE)
			return (blt_exit_and_free(all, 255, FALSE));
		return (blt_exit_and_free(all, ft_atoi(all->cmd[1]), TRUE));
	}
	return (blt_exit_and_free(all, errno, TRUE));
}

/*
** ---------------------------echo-----------------------------------------
*/

int		blt_echo(t_param *all)
{
	int		arg;
	int		option_n;

	if (!all->cmd[1])
		return (write(1, "\n", 1) - 1);
	arg = 1;
	option_n = FALSE;
	if (!ft_strcmp(all->cmd[1], "-n"))
		option_n = arg++;
	else if (check_options(all, 1) == TRUE)
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

/*
** ---------------------------unset-----------------------------------------
*/

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
	if (check_options(all, 1) == TRUE)
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
