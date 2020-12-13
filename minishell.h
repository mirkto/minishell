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
# include <stdio.h>// printf <-delete me
# include <strings.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>
# include "libs/libft/libft.h"

/*
** pid, ppid - 71
** fork + exec - 89
**
** <strings.h> strerror - 00
** <stdlib.h> malloc, free, exit
**				- 00, - 00, - 84
** <unistd.h> write, read, close, fork, getcwd, chdir, execve, dup, dup2, pipe
**			   - 52, - 52,  - 54, - 77,   - 00,  - 75,   - 80,   - 96,   - 120
** <fcntl.h> open - 50
** <sys/wait.h> wait, waitpid - 87
** <sys/types.h> wait3, wait4 - 88
** <signal.h> signal, kill - 109
** <sys/stat.h> stat, lstat, fstat - 61
** <dirent.h> opendir, readdir, closedir - 66
** <errno.h> errno - 00
*/

typedef struct		s_parameters
{
	int				i;
	char			*tmp;
	int				flag;
	int				buf_len;
	char			**env;
	char			**buf_lst;
	char			**pathes;
}					t_param;

int					parser(t_param *all, char **buf);

#endif
