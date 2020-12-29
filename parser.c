/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:55:10 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/29 16:43:57 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			quotes(char *tmp, int i)
{
	char	c;

	c = tmp[i];
	i++;
	while (tmp[i] != c)
	{
		if (c == '\"' && tmp[i] == '\\' && tmp[i + 1] != '\0')
			i++;
		else if (tmp[i] == '\0')
			return (i);
		i++;
	}
	return (i);
}

char		*token_init(t_param *all, char *tmp, int *i)
{
	int		end;

	end = *i;
	while (tmp[*i] != '\0')
	{
		if (tmp[*i] == '>' || tmp[*i] == '<' ||
			tmp[*i] == '|' || tmp[*i] == ';')
		{
			if (end == *i && tmp[*i + 1] == '>' && tmp[*i] == '>')
				(*i) += 2;
			else if (end == *i)
				(*i)++;
			break ;
		}
		if (tmp[*i] == ' ' || tmp[*i] == '\t')
			break ;
		if (tmp[*i] == '\\' && tmp[*i + 1] != '\0')
			(*i)++;
		else if ((tmp[*i] == '\"') || (tmp[*i] == '\''))
			*i = quotes(tmp, *i);
		(*i)++;
	}
	all->tok = ft_substr(tmp, end, *i - end);
	end = *i;
	return (all->tok);
}

char		**tmp_handler(t_param *all, char *tmp)
{
	int		i;
	char	**str;

	i = 0;
	while (tmp[i])
	{
		all->tok = token_init(all, tmp, &i);
		while (tmp[i] == ' ' || tmp[i] == '\t')
			i++;
		ft_lstadd_back(&all->vasya, ft_lstnew(all->tok));
		all->num_of_toks++;
	}
	if (!(str = (char **)malloc(sizeof(char *) * (all->num_of_toks + 1))))
		return (0);
	all->tmp_vasya = all->vasya;
	i = 0;
	while (all->vasya)
	{
		str[i] = ft_strdup(all->vasya->content);
		all->vasya = all->vasya->next;
		i++;
	}
	str[i] = NULL;
	return (str);
}

int			parser(t_param *all, char **buf)
{
	char	*tmp;
	char	**str;

	tmp = ft_strtrim(*buf, " \t\n");
	if (ft_strlen(tmp) <= 0)
	{
		free(tmp);
		return (-1);
	}
	if (lexer(tmp) == -1)
	{
		free(tmp);
		return (-1);
	}
	str = tmp_handler(all, tmp);
	// token_handler(str);
	ft_lstclear(&all->tmp_vasya, free);
	all->cmd = str;
	if (!all->cmd)
		ft_putendl("ERROR in process of split");
	free(tmp);
	return (0);
}
