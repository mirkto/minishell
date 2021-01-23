/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:58:02 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/21 17:37:35 by arannara         ###   ########.fr       */
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
# define OFLAG_TRUNC O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD
# define OFLAG_APPEND O_RDWR | O_CREAT | O_APPEND, S_IWRITE | S_IREAD

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
	char			*buf;
	int				buf_len;
	char			**env;
	char			**cmd;
	char			**pathes;
	int				num_of_toks;
	t_list			*vasya;
	t_list			*tmp_vasya;
	char			*tok;
	int				redirect;
	int				fd_1;
	int				fd_0;
	int				save_fd_1;
	int				save_fd_0;
	char			**cmd_tmp;
	char			*tmp3;
	int				pipe_num;
	int				semicolon_num;
	char			**pipes_remnant;
	char			**cmd_remnant;
	int				dollar_q;
	int				c;
	int				z;
}					t_param;

typedef struct		s_line
{
	char			*cmd_str;
	int				fd_input;
	int				fd_output;
}					t_line;

void				free_array(char **lst);
int					put_error(char *str_err, char *arg);
char				**split_pathes(t_param *all, char **env);
int					check_options(t_param *all, int i);
int					check_back_slash_n(t_param *all, char *tmp);

int					blt_exit(t_param *all);
int					blt_cd(t_param *all);
int					blt_pwd(t_param *all);
int					blt_unset(t_param *all);
int					blt_echo(t_param *all);

void				check_dollar(t_param *all);
int					blt_export_print(t_param *all);
int					blt_export_write(t_param *all, int i, int arg_n);
int					blt_export(t_param *all);
void		put_cmd(t_param *all);

char				**inc_env(char **env, char *str);
char				**copy_env(char **env, int len);
int					search_key_env(t_param *all, char *str);
char				*get_value_env(t_param *all, char *str);
int					blt_env(t_param *all);

int					exec_fork(t_param *all);
int					ft_execve(t_param *all);

int					parser(t_param *all, char **buf);
int					lexer(char *tmp);
char				**list_maker(t_param *all, char *tmp);
char				*str_joiner(char *tmp3, char *tok, int *i, int *z);
char				*quote_remover(int *i, char *tok, t_param *all);

void				handler_quit_(int);
void				handler_int_c(int);
void				handler_int_c_2(int i);

int					fd_close(int fd);
int					fd_check_and_dup(t_param *all);
int					fd_check_and_close(t_param *all);
int					cmd_remove_and_shift(t_param *all, int index, int num);
void				fd_processor(t_param *all, int i);

char				**split_loop(t_param *all, int i, char **cmd_tmp);
int					check_pipes(t_param *all);
int					check_semicolon(t_param *all);
int					split_by_semicolon(t_param *all);
int					check_and_replace_dollar_q(char **cmd);

int					pipe_conveyor(t_param *all);

char				*inits_buf_and_get_line(t_param *all, char *buf);
int					executor(t_param *all);

int					init_env_and_pathes(t_param *all, char **env);
void				inits_on_start_loop(t_param *all);

int					g_exit_code;
int					tmp_exit_code;

#endif
