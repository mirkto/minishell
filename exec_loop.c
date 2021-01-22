/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 13:59:11 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/21 17:38:11 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_pipes(t_param *all)
{
	int	i;

	i = -1;
	while (all->cmd[++i] != NULL)
		if (!ft_strcmp(all->cmd[i], "|"))
			all->pipe_num++;
	if (all->pipe_num == -1)
		return (1);
	all->pipe_num++;
	all->pipes_remnant = copy_env(all->cmd, 0);
	return (0);
}

int		split_by_pipes(t_param *all)
{
	int		i;
	char	**tmp_array;

	tmp_array = NULL;
	i = 0;
	while (all->pipes_remnant[i] != NULL)
	{
		if (!ft_strcmp(all->pipes_remnant[i], "|"))
		{
			free_array(all->cmd);
			all->cmd = copy_env(all->pipes_remnant, 0);
			all->i = 0;
			while (all->cmd[i + all->i])
				free(all->cmd[i + all->i++]);
			all->cmd[i] = NULL;

			tmp_array = copy_env(&all->pipes_remnant[i + 1], 0);
			free_array(all->pipes_remnant);
			all->pipes_remnant = tmp_array;
			return (1);
		}
		i++;
	}
	free_array(all->cmd);
	all->cmd = copy_env(all->pipes_remnant, 0);
	all->i = 0;
	while (all->cmd[i + all->i])
		free(all->cmd[i + all->i++]);
	all->cmd[i] = NULL;
	free_array(all->pipes_remnant);
	return (0);
}

int		check_semicolon(t_param *all)
{
	int	i;

	// if (!all->cmd)
	// 	return (-1);
	i = -1;
	while (all->cmd[++i] != NULL)
		if (!ft_strcmp(all->cmd[i], ";"))
			all->semicolon_num++;

	if (all->cmd[i] == NULL && !ft_strcmp(all->cmd[i - 1], ";"))
	{
		cmd_remove_and_shift(all, i - 1, 1);
		all->semicolon_num--;
	}
	all->cmd_remnant = copy_env(all->cmd, 0);
	return (0);
}

int		split_by_semicolon(t_param *all)
{
	int		i;
	int		j;
	char	**tmp_array;

	tmp_array = NULL;
	i = 0;
	while (all->cmd_remnant[i] != NULL)
	{
		if (!ft_strcmp(all->cmd_remnant[i], ";"))
		{
			free_array(all->cmd);
			all->cmd = copy_env(all->cmd_remnant, 0);
			j = 0;
			while (all->cmd[i + j])
				free(all->cmd[i + j++]);
			all->cmd[i] = NULL;

			tmp_array = copy_env(&all->cmd_remnant[i + 1], 0);
			free_array(all->cmd_remnant);
			all->cmd_remnant = tmp_array;
			return (1);
		}
		i++;
	}
	free_array(all->cmd);
	all->cmd = copy_env(all->cmd_remnant, 0);
	j = 0;
	while (all->cmd[i + j])
		free(all->cmd[i + j++]);
	all->cmd[i] = NULL;
	free_array(all->cmd_remnant);
	return (0);
}

int		check_and_replace_dollar_q(char **cmd)
{
	int		i;
	int		k;
	char	*tmp;
	char	*gec;
	char	*tmp_str;

	k = -1;
	while (cmd[++k] != NULL)
	{
		i = -1;
		while (cmd[k][++i])
			if (cmd[k][i] == '$' && cmd[k][i + 1] == '?')
			{
				tmp = ft_substr(*cmd, 0, i);
				gec = ft_itoa(g_exit_code);
				tmp_str = ft_strdup(&cmd[k][i + 2]);
				free(cmd[k]);
				cmd[k] = ft_strstrjoin(tmp, gec, tmp_str);
				free(gec);
				free(tmp_str);
				free(tmp);
			}
	}
	return (0);
}
// // ------------------
// int		paste_tmp_and_free(t_param *all)
// {
// 	if (all->cmd_flag == 1)
// 	{
// 		all->cmd_flag = 0;
// 		all->cmd = copy_env(all->cmd_tmp, 0);
// 		free_array(all->cmd_tmp);
// 		// ft_putendl("-tmp-");
// 		// put_cmd(all);
// 		// ft_putendl("-tmp-");
// 	}
// 	return (0);
// }

// int		cut_and_save_or_check_end(t_param *all, int *i)
// {
// 	if (all->cmd[*i + 1] == NULL)
// 	{
// 		free(all->cmd[*i]);
// 		all->cmd[*i] = NULL;
// 		return (1);
// 	}
// 	else
// 	{
// 		all->cmd_tmp = copy_env(&all->cmd[*i + 1], 0);
// 		all->i = 0;
// 		while (all->cmd[*i + all->i])
// 			free(all->cmd[*i + all->i++]);
// 		all->cmd[*i] = NULL;
// 	}
// 	all->cmd_flag = 1;
// 	*i = 0;
// 	return (0);
// }

// int		check_semicolon_and_pipe(t_param *all)
// {
// 	int	i;

// 	// ----print_cmd----
// 	ft_putendl("\n-str-");
// 	put_cmd(all);
// 	ft_putendl("-str-");
// 	// ----print_cmd----

// 	all->pipe_num = 0;
// 	i = -1;
// 	while (all->cmd[++i])
// 		if (!ft_strcmp(all->cmd[i], "|"))
// 			all->pipe_num += 1;
// 	ft_putnbr(all->pipe_num);//--
// 	ft_putendl("<-pipe_flag-\n");//--

// 	i = 0;
// 	while (1)
// 	{
// 		paste_tmp_and_free(all);
// 		if (all->cmd[i] == NULL)
// 		{
// 			if (all->pipe_num == 0)
// 				executor(all);
// 			free_array(all->cmd);
// 			return (0);
// 		}
// 		else if (!ft_strcmp(all->cmd[i], "|"))
// 		{
// 			if (cut_and_save_or_check_end(all, &i) == 0)
// 				pipe_conveyor(all);
// 			continue ;
// 		}
// 		else if (!ft_strcmp(all->cmd[i], ";"))
// 		{
// 			if (cut_and_save_or_check_end(all, &i) == 0)
// 				executor(all);
// 			continue ;
// 		}
// 		i++;
// 	}
// 	return (0);
// }
