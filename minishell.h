/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:58:02 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/28 15:39:18 by arannara         ###   ########.fr       */
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
# define ERROR -1
# define FALSE 0
# define TRUE 1
# define CONTINUE 2

/*
** <strings.h>	- strerror
** <stdlib.h>	- malloc, free, exit
** <unistd.h>	- write, read, close, fork, getcwd, chdir, execve, dup, pipe
** <fcntl.h>	- open
** <sys/wait.h>	- wait, waitpid
** <sys/types.h>- wait3, wait4
** <signal.h>	- signal, kill
** <sys/stat.h>	- stat, lstat, fstat
** <dirent.h>	- opendir, readdir, closedir
** <errno.h>	- errno
*/

typedef struct		s_parameters
{
	int				i;
	char			*tmp;
	int				flag;
	int				buf_len;
	char			**env;
	char			**cmd;
	char			**pathes;
	int				num_of_toks;
}					t_param;

typedef struct		s_line
{
	char			*cmd_str;
	int				fd_input;
	int				fd_output;
}					t_line;

void				free_array(char ***lst);
int					put_error(char *str_err, char *arg);
char				**split_pathes(t_param *all, char **env);
char				**inc_env(char ***env, char *str);
char				**copy_env(char **env, int len);
int					search_key_env(t_param *all, char *str);
char				*get_value_env(t_param *all, char *str);
int					check_options(t_param *all);

int					blt_exit(t_param *all);
int					blt_cd(t_param *all);
int					blt_pwd(t_param *all);
int					blt_env(t_param *all);
int					blt_unset(t_param *all);
int					blt_export(t_param *all);
int					blt_echo(t_param *all);

int					exec_fork(t_param *all);
int					ft_execve(t_param *all);

int					parser(t_param *all, char **buf);
int					lexer(char *tmp);

#endif
