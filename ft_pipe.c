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
	return (0);
}