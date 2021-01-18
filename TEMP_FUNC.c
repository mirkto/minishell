
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
** "\v\f\r \n\t"
**
** signal(SIGINT, handler);		// SIGINT - CTRL + C
** signal(SIGTERM, handler);
** signal(SIGKILL, handler);
** signal(SIGQUIT, handler);	// SIGQUIT - CTRL + \
**
** close(1);
** fd = open("file.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
**
** put_cmd(all);
**
** write(1, " | ", 3);
** ft_putstr(strerror(errno));
*/

// alias mm="make && ./minishell"

# include <unistd.h> // for NULL
# include <dirent.h> // for DIR
# include <errno.h>  // for errno

# include "minishell.h"

#define INPUT_CMD GREEN_INPUT RED_INPUT
#define GREEN_INPUT	GREEN INPUT RESET
#define GREEN	write(1, "\033[0;32m", 7);
#define INPUT	write(1, "minishell-0.0", 13);
#define RESET	write(1, "\033[0m", 4);
#define RED_INPUT write(1, "\033[0;31m$ \033[0m", 13);

int		wt(void)
{
	while (1)
		;
	return (0);
}

t_parse		*lst_new(char **arg);
t_parse		*lst_last(t_parse *lstarg);
void		add_back(t_parse **lstarg, t_parse *new);

typedef struct		s_parse
{
	char			**arg;
	struct s_parse	*next;
	struct s_parse	*prev;
}					t_parse;

t_parse		*lst_new(char **arg)
{
	t_parse	*tmp;

	tmp = (t_parse *)malloc(sizeof(t_parse));
	if (!tmp)
		return (NULL);
	tmp->arg = arg;
	tmp->next = NULL;
	tmp->prev = NULL;
	return (tmp);
}

t_parse		*lst_last(t_parse *lstarg)
{
	t_parse	*tmp;

	tmp = lstarg;
	if (tmp == NULL)
		return (NULL);
	while (tmp->next != NULL)
		tmp = tmp->next;
	return (tmp);
}

void		add_back(t_parse **lstarg, t_parse *new)
{
	t_parse	*last;
	t_parse *tmp;

	if (*lstarg == NULL)
		*lstarg = new;
	else
	{
		last = lst_last(*lstarg);
		last->next = new;
		tmp = last->next;
		tmp->prev = last;
	}
}

int		ft_check_dir(char *tmp, char *cmd)
{
	DIR *dir;
	struct dirent *read;
	
	dir = opendir(tmp);
	while (dir)
	{
		errno = 0;
		if ((read = readdir(dir)) != NULL)
		{
			if (ft_strcmp(read->d_name, cmd) == 0)
			{
				closedir(dir);
				return (1);
			}
		}
		else
		{
			if (errno == 0)
			{
				closedir(dir);
				return (0);
			}
			closedir(dir);
			return (-1);//READ_ERROR;
		}
	}
	return (-1);//OPEN_ERROR;
}
int		blt_env(t_param *all)
{
	if (all->cmd[1] && all->cmd[1][0] != '#' && !ft_strchr(all->cmd[1], '='))
	{
		if (all->cmd[1][0] == '-')
		{
			if (ft_isalpha(all->cmd[1][1]) == 1)
				return (put_error("Enter without any options!", NULL));
		}
		else
			return (put_error("No such file or directory", all->cmd[1]));
	}
	else
	{
		all->i = -1;
		while (all->env[++all->i])
			if (all->env[all->i] != NULL)
				if (ft_strchr(all->env[all->i], '='))
					ft_putendl(all->env[all->i]);
		if (all->cmd[1])
			if (ft_strchr(all->cmd[1], '='))
				ft_putendl(all->cmd[1]);
	}
	return (0);
}

// --------------signal-----------------

void	sig_int(int code)
{
	(void)code;
	if (g_sig.pid == 0)
	{
		ft_putstr_fd("\b\b  ", STDERR);
		ft_putstr_fd("\n", STDERR);
		ft_putstr_fd("\033[0;36m\033[1m🤬 minishell ▸ \033[0m", STDERR);
		g_sig.exit_status = 1;
	}
	else
	{
		ft_putstr_fd("\n", STDERR);
		g_sig.exit_status = 130;
	}
	g_sig.sigint = 1;
}

void	sig_quit(int code)
{
	char	*nbr;

	nbr = ft_itoa(code);
	if (g_sig.pid != 0)
	{
		ft_putstr_fd("Quit: ", STDERR);
		ft_putendl_fd(nbr, STDERR);
		g_sig.exit_status = 131;
		g_sig.sigquit = 1;
	}
	else
		ft_putstr_fd("\b\b  \b\b", STDERR);
	ft_memdel(nbr);
}

typedef struct		s_sig
{
	int				sigint;
	int				sigquit;
	int				exit_status;
	pid_t			pid;
}					t_sig;

void	sig_init(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
	g_sig.exit_status = 0;
}

// --------------------tmp_redirect-------------------

/*
typedef struct		s_var
{
	t_list			list;	// char			*cmd_str; использует листы вместо двумерного массива
	int				fd_1;	// input
	int				fd_0;	// output
	int				r;		// type of redirect: (1 это >) (2 это >>) (3 это <)
	int				exception; // пометка для пропуска этого листа по логике дальше
}					t_var;
*/
void	processing_fd(t_var *var, t_param *all)
{
									// int i = -1;
	if (var->list->next != NULL)	// while (all->cmd[++i] != NULL || != ";" || != "|")
	{
		var->list = var->list->next;	//

		if (all->fd_1 != -1 && (var->r == 1 || var->r == 2)) // ((all->cmd[i][0] == '>') || (all->cmd[i][0] == '>' && ll->cmd[i][1] == '>'))
			all->fd_1 = close_fd(all->fd_1);
		else if (all->fd_0 != -1 && var->r == 3) // (all->cmd[i][0] == '<')
			all->fd_1 = close_fd(all->fd_1);

		if (var->r == 1)		// > // if (all->cmd[i][0] == '>')
			all->fd_1 = open(var->list->content, O_RDWR | O_CREAT | \ // all->cmd[++i]
			O_TRUNC, S_IWRITE | S_IREAD);
		else if (var->r == 2)	// >> // if (all->cmd[i][0] == '>' && ll->cmd[i][1] == '>')
			all->fd_1 = open(var->list->content, O_RDWR | O_CREAT | \ // all->cmd[++i]
			O_APPEND, S_IWRITE | S_IREAD);

		else if (var->r == 3)	// < // if (all->cmd[i][0] == '<')
			all->fd_0 = open(var->list->content, O_RDONLY); // all->cmd[++i]

		// var->exception = 1;
		// permission_denied(all);
	}
}

void	fd_processor(t_param *all)
{
	int	i;

	i = 0;
	while (all->cmd[i] != NULL)
	{
		if ((all->cmd[i][0] == ';' && all->cmd[i][1] == '\0') ||
			(all->cmd[i][0] == '|' && all->cmd[i][1] == '\0'))
			break ;
		if (all->fd_1 != -1 && \
				((all->cmd[i][0] == '>' && all->cmd[i][1] == '\0') || \
				(all->cmd[i][0] == '>' && all->cmd[i][1] == '>' && \
											all->cmd[i][2] == '\0')))
			all->fd_1 = fd_close(all->fd_1);
		else if (all->fd_0 != -1 && \
					(all->cmd[i][0] == '<' && all->cmd[i][2] == '\0'))
			all->fd_1 = fd_close(all->fd_1);
		if (all->cmd[i][0] == '>' && all->cmd[i][1] == '\0')
		{
			all->fd_1 = open(all->cmd[i + 1], O_RDWR | O_CREAT | \
									O_TRUNC, S_IWRITE | S_IREAD);
			all->redirect = cmd_remove_and_shift(all, i, 2);
		}
		else if (all->cmd[i][0] == '>' &&
				all->cmd[i][1] == '>' && all->cmd[i][2] == '\0')
		{
			all->fd_1 = open(all->cmd[i + 1], O_RDWR | O_CREAT | \
									O_APPEND, S_IWRITE | S_IREAD);
			all->redirect = cmd_remove_and_shift(all, i, 2);
		}
		else if (all->cmd[i][0] == '<' && all->cmd[i][1] == '\0')
		{
			all->fd_0 = open(all->cmd[i + 1], O_RDONLY);
			all->redirect = cmd_remove_and_shift(all, i, 2);
		}
		else
			i++;
	}
}

// -------------------pipe-----------------------------

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

	// all->pipe_flag == 1;
					// if (all->pipe_flag == 1)
				// 	executor(&all);
		// int				pipe_flag;
		int					fd_close(int fd);