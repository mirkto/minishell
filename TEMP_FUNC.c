
/*
pid, ppid - 71
fork + exec - 89

<strings.h> strerror - 00
<stdlib.h> malloc, free, exit
		   \___00, \_00, \_84
<unistd.h> write, read, close, fork, getcwd, chdir, execve, dup, dup2, pipe
		   \__52, \_52, \__54, \_77, \___00, \__75, \___80, \______96, \120
<fcntl.h> open - 50
<sys/wait.h> wait, waitpid - 87
<sys/types.h> wait3, wait4 - 88
<signal.h> signal, kill - 109
<sys/stat.h> stat, lstat, fstat - 61
<dirent.h> opendir, readdir, closedir - 66
<errno.h> errno - 00

ft_putendl("---");
ft_putnbr(len);
ft_putendl("");
write(1, "\n", 1);
ft_putendl(all->env[all->i]);

"\v\f\r \n\t"

signal(SIGINT, handler);	// SIGINT - CTRL + C
signal(SIGTERM, handler);
signal(SIGKILL, handler);
signal(SIGQUIT, handler);	// SIGQUIT - CTRL + \

close(1);
fd = open("file.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);

put_cmd(all);

write(1, " | ", 3);
ft_putstr(strerror(errno));

// alias mm="make && ./minishell"
*/
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
// --------------------not_end_loop-------------------
int		wt(void)
{
	while (1)
		;
	return (0);
}
// --------------------list-------------------
/*
t_parse		*lst_new(char **arg);
t_parse		*lst_last(t_parse *lstarg);
void		add_back(t_parse **lstarg, t_parse *new);
*/
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
// --------------------old_dir-------------------
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
// ----------------pipes_conveyor-------------------
void connect_stdio_to_pipes(int prev_fds[], int next_fds[])
{
	if (prev_fds[0] >= 0)
	{
		dup2(prev_fds[0], 0);
		close(prev_fds[0]);
		close(prev_fds[1]);
	}
	if (next_fds[1] >= 0)
	{
		dup2(next_fds[1], 1);
		close(next_fds[1]);
		close(next_fds[0]);
	}
}

int main(int argc, char **argv, char **envp)
{
	char *cmd1[] = { "/bin/cat", NULL };
	char *cmd2[] = { "/usr/bin/grep", "a", NULL };
	char *cmd3[] = { "/usr/bin/grep", "a", NULL };
	char *cmd4[] = { "/usr/bin/grep", "a", NULL };
	char **cmds[5];
	cmds[0] = cmd1;
	cmds[1] = cmd2;
	cmds[2] = cmd3;
	cmds[3] = cmd4;
	cmds[4] = NULL;

	int i = 0;
	int cmds_count = 4;
	int prev_pipe_fds[2];
	int next_pipe_fds[2];
	next_pipe_fds[0] = -1;
	next_pipe_fds[1] = -1;
	while (i < cmds_count)
	{
		prev_pipe_fds[0] = next_pipe_fds[0];
		prev_pipe_fds[1] = next_pipe_fds[1];
		if (i != cmds_count - 1)
		{
			pipe(next_pipe_fds);
			printf("%d, %d\n", next_pipe_fds[0], next_pipe_fds[1]);
		}
		else
		{
		next_pipe_fds[0] = -1;
		next_pipe_fds[1] = -1;
		}
		if (fork() == 0)
		{
			connect_stdio_to_pipes(prev_pipe_fds, next_pipe_fds);
			char **cmd = cmds[i];
			execve(cmd[0], cmd, NULL);
		}
		close(prev_pipe_fds[0]);
		close(prev_pipe_fds[1]);
		i++;
	}
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	return (0);
}
// ----pff-----------
int		pipe_conveyor_one(t_param *all, int fd[2])
{
	if (fork() == 0)
	{
		// ft_putendl("-1-");
		// int i = -1;
		// while (all->cmd[++i] != NULL)
		// 	ft_putendl(all->cmd[i]);

		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		fd_processor(all);
		ft_execve(all);
		// executor(all);
		exit (1);
	}
	return (0);
}

int		pipe_conveyor_two(t_param *all, int fd[2])
{
	if (fork() == 0)
	{
		// ft_putendl("-2-");
		// int i = -1;
		// while (all->cmd[++i] != NULL)
		// 	ft_putendl(all->cmd[i]);

		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		fd_processor(all);
		ft_execve(all);
		// executor(all);
		exit (1);
	}
	return (0);
}

int		check_tmp_on_end(t_param *all)
{
	int		i;
	char	**tmp;
	
	i = 0;
	tmp = NULL;
	while (1)
	{
		
		if (all->cmd_tmp[i + 1] == NULL)
		{
			free_array(all->cmd);
			all->cmd = copy_env(all->cmd_tmp, 0);
			// free_array(all->cmd_tmp);
			break ;
		}
		if (!ft_strcmp(all->cmd_tmp[i], ";") || !ft_strcmp(all->cmd_tmp[i], "|"))
		{
			tmp = copy_env(&all->cmd_tmp[i + 1], 0);
			all->i = 0;
			while (all->cmd_tmp[i + all->i])
				free(all->cmd_tmp[i + all->i++]);
			all->cmd_tmp[i] = NULL;
			free_array(all->cmd);
			all->cmd = copy_env(all->cmd_tmp, 0);
			if (tmp != NULL)
			{
				free_array(all->cmd_tmp);
				all->cmd_tmp = copy_env(tmp, 0);
				free_array(tmp);
			}
			// free_array(all->cmd_tmp);
			break ;
		}
		i++;
	}
		return (0);
}