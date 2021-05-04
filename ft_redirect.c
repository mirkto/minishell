#include "minishell.h"

int		fd_close(int fd)
{
	close(fd);
	fd = -2;
	return (fd);
}

int		fd_check_and_dup(t_param *all)
{
	if (all->redirect == 1)
	{
		all->save_fd_1 = dup(1);
		all->save_fd_0 = dup(0);
	}
	if (all->fd_1 != -2)
		dup2(all->fd_1, 1);
	if (all->fd_0 != -2)
		dup2(all->fd_0, 0);
	return (0);
}

int		fd_check_and_close(t_param *all)
{
	if (all->redirect == 1)
	{
		dup2(all->save_fd_1, 1);
		dup2(all->save_fd_0, 0);
	}
	if (all->fd_1 != -2)
		all->fd_1 = fd_close(all->fd_1);
	if (all->fd_0 != -2)
		all->fd_0 = fd_close(all->fd_0);
	if (all->save_fd_1 != -2)
		all->save_fd_1 = fd_close(all->save_fd_1);
	if (all->save_fd_0 != -2)
		all->save_fd_0 = fd_close(all->save_fd_0);
	return (0);
}

int		cmd_remove_and_shift(t_param *all, int index, int num)
{
	int	tmp;

	tmp = index;
	while (num > 0)
	{
		free(all->cmd[index]);
		while (all->cmd[index])
		{
			all->cmd[index] = all->cmd[index + 1];
			index++;
		}
		index = tmp;
		num--;
	}
	return (1);
}

void	fd_processor(t_param *all, int i)
{
	while (all->cmd[i] != NULL)
	{
		if (all->fd_1 != -2 &&
		(!ft_strcmp(all->cmd[i], ">") || !ft_strcmp(all->cmd[i], ">>")))
			all->fd_1 = fd_close(all->fd_1);
		else if (all->fd_0 != -2 && !ft_strcmp(all->cmd[i], "<"))
			all->fd_1 = fd_close(all->fd_1);
		if (!ft_strcmp(all->cmd[i], ">") || !ft_strcmp(all->cmd[i], ">>") ||
			!ft_strcmp(all->cmd[i], "<"))
		{
			if (!ft_strcmp(all->cmd[i], ">"))
				all->fd_1 = open(all->cmd[i + 1], OFLAG_TRUNC);
			else if (!ft_strcmp(all->cmd[i], ">>"))
				all->fd_1 = open(all->cmd[i + 1], OFLAG_APPEND);
			else if (!ft_strcmp(all->cmd[i], "<"))
				all->fd_0 = open(all->cmd[i + 1], O_RDONLY);
			if (all->fd_1 == -1 || all->fd_0 == -1)
				all->redirect = cmd_remove_and_shift(all, i, 1);
			else
				all->redirect = cmd_remove_and_shift(all, i, 2);
		}
		else
			i++;
	}
}
