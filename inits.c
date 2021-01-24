/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 17:58:56 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/23 17:58:59 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		init_env_and_pathes(t_param *all, char **env)
{
	all->env = copy_env(env, 0);
	if (search_key_env(all, "OLDPWD") == -1)
		all->env = inc_env(all->env, "OLDPWD");
	all->pathes = split_pathes(all, env);
	g_tmp_exit_code = 0;
	all->fd_0 = -2;
	all->fd_1 = -2;
	all->save_fd_0 = -2;
	all->save_fd_1 = -2;
	all->dollar_q = 0;
	return (0);
}

void	inits_on_start_loop(t_param *all)
{
	all->i = 0;
	all->tmp = NULL;
	all->flag = 0;
	all->buf_len = 0;
	all->cmd = NULL;
	all->num_of_toks = 0;
	all->vasya = NULL;
	all->tok = NULL;
	g_exit_code = g_tmp_exit_code;
	g_tmp_exit_code = 0;
	signal(SIGINT, handler_int_c);
	signal(SIGQUIT, handler_quit_);
	all->redirect = 0;
	all->cmd_tmp = NULL;
	all->semicolon_num = 0;
	all->pipe_num = -1;
}
