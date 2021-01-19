/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 13:59:11 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/19 13:59:25 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		paste_tmp_and_free(t_param *all)
{
	if (all->cmd_flag == 1)
	{
		all->cmd_flag = 0;
		all->cmd = copy_env(all->cmd_tmp, 0);
		free_array(&all->cmd_tmp);
		// ft_putendl("-tmp-");
		// put_cmd(all);
		// ft_putendl("-tmp-");
	}
	return (0);
}

int		cut_and_save_or_check_end(t_param *all, int *i)
{
	if (all->cmd[*i + 1] == NULL)
	{
		free(all->cmd[*i]);
		all->cmd[*i] = NULL;
		return (1);
	}
	else
	{
		all->cmd_tmp = copy_env(&all->cmd[*i + 1], 0);
		all->i = 0;
		while (all->cmd[*i + all->i])
			free(all->cmd[*i + all->i++]);
		all->cmd[*i] = NULL;
	}
	all->cmd_flag = 1;
	*i = 0;
	return (0);
}

int		check_semicolon_and_pipe(t_param *all)
{
	int	i;

	ft_putendl("\n-str-");
	put_cmd(all);
	ft_putendl("-str-\n");
	i = -1;
	while (all->cmd[++i])
		if (!ft_strcmp(all->cmd[i], "|"))
			all->pipe_flag += 1;
	ft_putnbr(all->pipe_flag);
	ft_putendl("---");
	i = 0;
	while (1)
	{
		paste_tmp_and_free(all);
		if (all->cmd[i] == NULL)
		{
			// ft_putendl("not find '|' or ';'");
			return (executor(all));
		}
		else if (!ft_strcmp(all->cmd[i], "|"))
		{
			// // ft_putendl("find '|'");
			// if (cut_and_save_or_check_end(all, i) == 1)
			// 	break ;
			// pipe_conveyor(all);
			// // all->pipe_flag = 0;
			// return (1);
			// // executor(all);
			// // continue ;
			if (cut_and_save_or_check_end(all, &i) == 0)
			{
				pipe_fd_open(all);
				// all->cmd_flag = 0;
				// pipe_conveyor(all);
				// executor(all);
				// return (0);
				continue ;
			}
			continue ;
		}
		else if (!ft_strcmp(all->cmd[i], ";"))
		{
			if (cut_and_save_or_check_end(all, &i) == 0)
				executor(all);
			continue ;
		}
		i++;
	}
	return (0);
}
