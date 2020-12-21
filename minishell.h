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
# include "libft/libft.h"

# define BUFFER_SIZE 1024

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
**
** ft_putendl("---");
** ft_putnbr(len);
** ft_putendl("");
** write(1, "\n", 1);
** ft_putendl(all->env[all->i]);
**
** close(1);
** fd = open("file.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
*/

typedef struct		s_parameters
{
	int				i;
	char			*tmp;
	int				flag;
	int				buf_len;
	char			**env;
	char			**cmd_lst;
	char			**pathes;
}					t_param;

typedef struct		s_line
{
	char			*cmd_str;
	int				fd_input;
	int				fd_output;
}					t_line;

int					parser(t_param *all, char **buf);

void	free_buf(char ***lst);
int		error_out(char *str_err, char *arg);
char	**init_pathes(t_param *all, char **env);
char	**copy_env(char **env, int len);

int		blt_exit();
int		blt_cd(t_param *all);
int		blt_pwd(t_param *all);
int		blt_env(t_param *all, int flag);
int		blt_unset(t_param *all);
int		blt_export(t_param *all);
int		blt_echo(t_param *all);

int		ft_execve(t_param *all);

#endif

