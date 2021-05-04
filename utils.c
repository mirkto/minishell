#include "minishell.h"

void	free_array(char **lst)
{
	int		i;

	i = 0;
	while (lst[i])
	{
		free(lst[i]);
		lst[i] = NULL;
		i++;
	}
	free(lst);
	lst = NULL;
}

int		put_error(char *str_error, char *arg)
{
	if (arg == NULL)
		ft_putendl(str_error);
	else
	{
		write(1, "minishell: ", 11);
		if (arg[0] == '@' && arg[1] == '#' && arg[2] == '&')
		{
			ft_putstr(&arg[3]);
			free(arg);
		}
		else
			ft_putstr(arg);
		write(1, ": ", 2);
		ft_putstr(str_error);
		write(1, "\n", 1);
	}
	g_exit_code = 1;
	return (-1);
}

char	**split_pathes(t_param *all, char **env)
{
	char	**tmp;
	int		i;

	tmp = NULL;
	i = -1;
	while (env[++i])
		if (!ft_strncmp(env[i], "PATH=", 5))
		{
			all->tmp = ft_strdup(&env[i][5]);
			tmp = ft_split(all->tmp, ':');
			free(all->tmp);
			break ;
		}
	return (tmp);
}

int		check_options(t_param *all, int arg_n)
{
	if (all->cmd[arg_n])
		if (all->cmd[arg_n][0] == '-' && ft_isalpha(all->cmd[arg_n][1]) == TRUE)
			return (TRUE);
	return (FALSE);
}

void	check_dollar(t_param *all)
{
	int		word;
	int		c;

	word = 0;
	while (all->cmd[++word])
	{
		c = 0;
		all->i = 0;
		all->tmp = ft_strdup(all->cmd[word]);
		while (all->cmd[word][c])
		{
			while (all->cmd[word][c] == '$')
			{
				c++;
				while (ft_isalpha(all->cmd[word][c]) == TRUE)
					c++;
				if (ft_isdigit(all->cmd[word][c]) == TRUE)
					c++;
			}
			all->tmp[all->i++] = all->cmd[word][c++];
		}
		all->tmp[all->i] = '\0';
		free(all->cmd[word]);
		all->cmd[word] = all->tmp;
	}
}
