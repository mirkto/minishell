/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 18:24:36 by ngonzo            #+#    #+#             */
/*   Updated: 2020/11/27 18:24:40 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define INPUT_CMD GREEN_INPUT RED_INPUT
#define GREEN_INPUT	GREEN INPUT RESET
#define GREEN	write(1, "\033[0;32m", 7);
#define INPUT	write(1, "minishell-0.0", 13);
#define RESET	write(1, "\033[0m", 4);
#define RED_INPUT write(1, "\033[0;31m$ \033[0m", 13);

void	error_out(char *str, t_param *all)
{
	write(1, "minishell: ", 11);
	ft_putstr(all->buf_lst[0]);
	write(1, ": ", 2);
	ft_putstr(str);
	write(1, "\n", 1);
}

int		main(void)
{
	char	*buf;
	t_param	all;

	while (1)
	{
		write(1, "\033[0;32mminishell-0.1$ \033[0m", 26);
		buf = (char *)malloc(sizeof(char) * 99);
		ft_bzero(buf, 99);
		read(0, buf, 99);
		// ------------------------------------
		parser(&all, &buf);

		if (all.buf_len < 1 || all.commas == -1)
		{
			if (all.commas == -1)
				ft_putendl("odd number of commas, multiline commands are forbidden");
			free(buf);
			continue;
		}
		else if (!ft_strncmp(all.buf_lst[0], "q", 2) || !ft_strncmp(all.buf_lst[0], "exit", 5))
		{
			write(1, "exit\n", 5);
			exit(0);
		}
		else if (!ft_strncmp(all.buf_lst[0], "pwd", 4))
		{
			char *dir = getcwd(NULL, 0);
			ft_putendl(dir);
			free(dir);
		}
		else if (!ft_strncmp(all.buf_lst[0], "env", 4))
		{
			all.i = -1;
			while (environ[++all.i])
				ft_putendl(environ[all.i]);
		}
		else if (!ft_strncmp(all.buf_lst[0], "cd", 3))
		{
			if (chdir(all.buf_lst[1]) < 0)
				error_out("No such file or directory", &all);
		}
		else
		{
			error_out("command not found", &all);
		}
		// --------------print_buf---------------
		// all.i = 0;
		// while (all.buf_lst[all.i])
		// {
		// 	ft_putendl(all.buf_lst[all.i]);
		// 	all.i++;
		// }
		// --------------------------------------
		free_buf_lst(&all);
		free(buf);
	}
	return (0);
}
