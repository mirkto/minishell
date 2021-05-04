#include "minishell.h"

int		exec_fork(t_param *all)
{
	int		status;
	int		wr;

	signal(SIGINT, handler_int_c_2);
	status = fork();
	if (status == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		fd_check_and_dup(all);
		if (execve(all->tmp, all->cmd, all->env) == -1)
		{
			put_error("command not found", all->cmd[0]);
			exit(127);
		}
	}
	wr = wait(&status);
	fd_check_and_close(all);
	if (wr != -1)
	{
		g_tmp_exit_code = WEXITSTATUS(status);
		return (1);
	}
	return (0);
}

int		check_command_name(char *tmp, char *cmd)
{
	char			*file_name;
	struct stat		buf;
	int				check;

	file_name = ft_strjoin(tmp, cmd);
	check = stat(file_name, &buf);
	free(file_name);
	if (check == 0)
		return (0);
	return (-1);
}

int		exec_check_path(t_param *all)
{
	char	*tmp;
	int		i;

	if (search_key_env(all, "PATH") == -1)
		return (put_error("No such file or directory", all->cmd[0]));
	free_array(all->pathes);
	all->pathes = split_pathes(all, all->env);
	i = -1;
	while (all->pathes[++i])
	{
		tmp = ft_strjoin(all->pathes[i], "/");
		if (check_command_name(tmp, all->cmd[0]) == 0)
		{
			all->tmp = ft_strjoin(tmp, all->cmd[0]);
			exec_fork(all);
			free(all->tmp);
			free(tmp);
			return (1);
		}
		free(tmp);
	}
	return (0);
}

int		exec_absolute_and_relative_path(t_param *all)
{
	int	flag;

	flag = 0;
	all->tmp = ft_strdup(all->cmd[0]);
	flag = exec_fork(all);
	free(all->tmp);
	return (flag);
}

int		ft_execve(t_param *all)
{
	int	flag;

	flag = 0;
	if (all->cmd[0][0] == '/' || all->cmd[0][0] == '.')
		flag = exec_absolute_and_relative_path(all);
	else
		flag = exec_check_path(all);
	if (flag == 0)
	{
		if (all->pipes_fd == 1)
		{
			dup2(all->input_fd_1, 1);
			dup2(all->output_fd_0, 0);
		}
		g_tmp_exit_code = 127;
		put_error("command not found", all->cmd[0]);
	}
	return (0);
}
