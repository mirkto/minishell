/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 20:50:08 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/13 20:50:21 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		close_fd(int fd)
{
	close(fd);
	fd = -1;
	return (fd);
}
/*
typedef struct		s_var
{
	t_list			list;	// char			*cmd_str; использует листы вместо двумерного массива
	int				fd_1;	// input
	int				fd_0;	// output
	int				r;		// type of redirect: (1 это >) (2 это >>) (3 это <)
	int				exception; // пометка для пропуска этого листа по логике дальше
}					t_var;

typedef struct		s_commands // это Анина структура
{
	int				fd_1;	// input
	int				fd_0;	// output
}					t_commands;
*/
void	processing_fd(t_var *var, t_commands *cmd) // вторая это Анина структура
{
	if (var->list->next != NULL)
	{
		var->list = var->list->next;
		if (cmd->fd_1 != -1 && (var->r == 1 || var->r == 2))
			cmd->fd_1 = close_fd(cmd->fd_1);
		else if (cmd->fd_0 != -1 && var->r == 3)
			cmd->fd_1 = close_fd(cmd->fd_1);
		if (var->r == 1)
			cmd->fd_1 = open(var->list->content, O_RDWR | O_CREAT | \
			O_TRUNC, S_IWRITE | S_IREAD);
		else if (var->r == 2)
			cmd->fd_1 = open(var->list->content, O_RDWR | O_CREAT | \
			O_APPEND, S_IWRITE | S_IREAD);
		else if (var->r == 3)
			cmd->fd_0 = open(var->list->content, O_RDONLY);
		var->exception = 1;
		// permission_denied(cmd); // ничего не знаю это делала Аня
	}
}
