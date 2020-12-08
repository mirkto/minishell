/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:58:02 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/02 16:58:04 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// pid, ppid - 71
// env, getenv, setenv, unsetenv - 74
// fork + exec - 89
#include <stdio.h> // printf <-delete me
#include <strings.h> // strerror - 00
#include <stdlib.h> // malloc, free, exit
					//   - 00, - 00, - 84
#include <unistd.h> // write, read, close, fork, getcwd, chdir, execve, dup, dup2, pipe
					// - 52, - 52,   - 54, - 77,   - 00,  - 75,   - 80,   - 96,   - 120
#include <fcntl.h> // open - 50
#include <sys/wait.h> // wait, waitpid - 87
#include <sys/types.h> // wait3, wait4 - 88
#include <signal.h> // signal, kill - 109
#include <sys/stat.h> // stat, lstat, fstat - 61
#include <dirent.h> // opendir, readdir, closedir - 66
#include <errno.h> // errno - 00
#include "libs/libft/libft.h"

extern char **environ;

typedef struct		s_parameters
{
	int				i;
	int				commas;
	int				buf_len;
	char			**buf_lst;
}					t_param;

void		free_buf_lst(t_param *all);
int			parser(t_param *all, char **buf);

#endif