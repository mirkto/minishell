
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
