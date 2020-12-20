
#define INPUT_CMD GREEN_INPUT RED_INPUT
#define GREEN_INPUT	GREEN INPUT RESET
#define GREEN	write(1, "\033[0;32m", 7);
#define INPUT	write(1, "minishell-0.0", 13);
#define RESET	write(1, "\033[0m", 4);
#define RED_INPUT write(1, "\033[0;31m$ \033[0m", 13);

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