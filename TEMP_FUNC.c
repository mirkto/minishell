
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
** signal(SIGKILL, handler);	// SIGKILL - CTRL + D
** signal(SIGQUIT, handler);	// SIGQUIT - CTRL + \
**
** close(1);
** fd = open("file.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
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
