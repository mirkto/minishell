/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:55:10 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/22 22:26:08 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*join_and_zi(char *str, char *tok, int *i, int *z)
{
	char	*tmp;

	tmp = str_joiner(str, tok, i, z);
	*z = *i;
	return (tmp);
}

char		*dollar_handler(t_param *all, char *tok, int *i, char *str)
{
	char	*tmp;
	char	*tmp2;

	tmp = str;
	str = get_value_env(all, &tok[(*i) + 1]);
	tmp2 = ft_strjoin(tmp, str);
	free(str);
	free(tmp);
	str = tmp2;
	return (str);
}

char		*slash_remover(int *i, char *tok, t_param *all)
{
	char	*tmp3;

	all->c = tok[(*i)++];
	all->z = *i;
	tmp3 = ft_calloc(sizeof(char), 2);
	all->flag = 0;
	while (tok[*i] && tok[*i] != all->c)
		(*i)++;
	if (all->z != (*i))
		tmp3 = str_joiner(tmp3, tok, i, &all->z);
	(*i)++;
	return (tmp3);
}

char		*token_handler(t_param *all, char *tok)
{
	char	*str;
	int		i;

	i = 0;
	all->z = 0;
	str = ft_calloc(sizeof(char), 3);
	while (tok[i])
	{
		if (tok[i] == '\\')
		{
			if (all->z != i)
				str = join_and_zi(str, tok, &i, &all->z);
			if (tok[i] == '\'' || tok[i] == '\"')
				slash_processing(str, tok, &i, all);
			else
				i++;
		}
		else if (tok[i] == '>' || tok[i] == '<'
				|| tok[i] == ';' || tok[i] == '|')
		{
			i--;
			while (tok[++i])
				str[i] = tok[i];
			return (str);
		}
		else
		{
			if (all->z != i)
				str = join_and_zi(str, tok, &i, &all->z);
			if (tok[i] == '\'' || tok[i] == '\"')
				quote_processing(str, tok, &i, all);
			else
				i++;
		}
	}
	if (all->z != i)
		str = str_joiner(str, tok, &i, &all->z);
	return (str);
}

int			parser(t_param *all, char **buf)
{
	char	*tmp;
	char	**str;
	char	*str2;

	tmp = ft_strtrim(*buf, " \t\n");
	if ((ft_strlen(tmp) <= 0) || (lexer(tmp) == -1))
	{
		free(tmp);
		free(all->buf);
		return (-1);
	}
	str = list_maker(all, tmp);
	all->i = -1;
	while (str[++all->i])
	{
		str2 = token_handler(all, str[all->i]);
		free(str[all->i]);
		str[all->i] = ft_strdup(str2);
		free(str2);
	}
	ft_lstclear(&all->tmp_vasya, free);
	all->cmd = str;
	free(tmp);
	free(all->buf);
	return (0);
}
