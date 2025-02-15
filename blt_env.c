#include "minishell.h"

char	**inc_env(char **env, char *str)
{
	char	**tmp;
	int		len;

	len = 0;
	while (env[len])
		len++;
	tmp = copy_env(env, len + 1);
	free_array(env);
	tmp[len] = ft_strdup(str);
	tmp[len + 1] = NULL;
	return (tmp);
}

char	**copy_env(char **env, int len)
{
	char	**new_env;
	int		i;

	if (len == 0)
	{
		while (env[len] != NULL)
			len++;
	}
	if (!(new_env = (char **)malloc(sizeof(char *) * (len + 1))))
	{
		put_error("dont work malloc copy env", "Error");
		return (NULL);
	}
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int		search_key_env(t_param *all, char *str)
{
	int		index;
	int		len;

	if (str == NULL)
		len = 0;
	else
		len = ft_strlen(str);
	if (len == 0)
		return (-1);
	index = 0;
	while (all->env[index] != NULL)
	{
		if (ft_strncmp(all->env[index], str, len) == 0)
			if (all->env[index][len] == '=' || all->env[index][len] == '\0')
				return (index);
		index++;
	}
	return (-1);
}

char	*get_value_env(t_param *all, char *str)
{
	char	*tmp;
	int		index;
	int		len;

	tmp = NULL;
	len = ft_strlen(str);
	index = search_key_env(all, str);
	if (index == -1)
		tmp = ft_strdup("");
	else if (all->env[index][len] == '=')
		tmp = ft_strdup(&all->env[index][++len]);
	return (tmp);
}

int		blt_env(t_param *all)
{
	if (all->cmd[1])
		return (put_error("Enter without any options or arguments!", NULL));
	all->i = -1;
	while (all->env[++all->i])
		if (all->env[all->i] != NULL)
			if (ft_strchr(all->env[all->i], '='))
				ft_putendl(all->env[all->i]);
	return (0);
}
