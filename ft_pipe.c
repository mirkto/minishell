/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 20:45:26 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/13 20:45:29 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_pipes_and_end(t_param *all)
{
	int	i;

	i = 0;
	while (1)
	{
		if (all->cmd_flag == 1)
		{
			all->cmd_flag = 0;
			all->cmd = copy_env(all->cmd_tmp, 0);
			free_array(&all->cmd_tmp);
			// put_cmd(all);
		}
		if (all->cmd[i] == NULL)
		{
			// ft_putendl("not find '|' or ';'");
			break ;
		}
		else if (!ft_strcmp(all->cmd[i], "|"))
		{
			ft_putendl("find '|'");
			return (1);
		}
		else if (!ft_strcmp(all->cmd[i], ";"))
		{
			// ft_putendl("not find ';'");
			i++;
			all->cmd_tmp = copy_env(&all->cmd[i], 0);
			i--;
			all->i = 0;
			while (all->cmd[i + all->i])
			{
				free(all->cmd[i + all->i]);
				all->i++;
			}
			all->cmd[i] = NULL;
			// put_cmd(all);
			all->cmd_flag = 1;
			return (2);
		}
		i++;
	}
	executor(all);
	return (0);
}

int		ft_pipe(t_param *all)
{
	all->i = 0;
	int	rc;

	int		fd[2];
	int		p1;
	int		p2;

	pipe(fd);
	p1 = fork();
	if (p1 == 0) // child #1
	{
		close(fd[0]);
		// ...
		write(fd[1], "...", 3);
		// ...
		exit (0);
	}
	p2 = fork();
	if (p2 == 0) // child #2
	{
		close (fd[1]);
		// ...
		rc = read(fd[0], "...", 3);
		// ...
		exit(0);
	}
	// parent
	close(fd[0]);
	close(fd[1]);
	// ...
	return (0);
}

int		conveyor(t_param *all)
{
	all->i = 0;

	int		fd[2];		// переменная для хранения ввода и вывода в канале связи
						// (fd[0] - output, fd[1] - input)

	pipe(fd);			// создаем канал связи (create pipe)
	if (fork() == 0)	// процесс для выполнения первой команды
	{
		close(fd[0]);	// читать из канала не нужно (output close) 
		dup2(fd[1], 1);	// подаем стандартный вывод - в канал связи (input -> pipe)
		close(fd[1]);	// закрываем после передачи в канал (input close)
		execlp("ls", "ls", "-lR", NULL);	// запускаем первую команду (execve1 run)
		perror("ls");	// если команда не закрылась сама, значит произошла ошибка
		exit(1);		// мы сообщаем об этом и закрываем вручную
	}
	if (fork() == 0)	// процесс для выполнения второй команды
	{
		close(fd[1]);	// писать в канал не нужно (input close)
		dup2(fd[0], 0);	// принимаем стандартный ввод - из канала связи (output <- pipe)
		close(fd[0]);	// закрываем после принятия из канала (output close)
		execlp("grep", "grep", "^d", NULL);	// запускаем вторую команду (execve2 run)
		perror("grep");	// если команда не закрылась сама, значит произошла ошибка
		exit(1);		// мы сообщаем об этом и закрываем вручную
	}
	close(fd[0]);	// в родительском процессе закрываем оба канала (output close)
	close(fd[1]);	// (input close)
	wait(NULL);		// и дожидаемся завершения двух дочерних процессов (wait execve1)
	wait(NULL);		// (wait execve2)
	return (0);
}