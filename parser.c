/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:55:10 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/27 20:46:37 by arannara         ###   ########.fr       */
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

char		*token_init(char *tmp, int *i)
{
	char	*tok;
	int		end;

	end = *i;
	while (tmp[*i] != '\0' && tmp[*i] != ' ' && tmp[*i] != '\t')
	{
		if (tmp[*i] == '\\' && tmp[*i + 1] != '\0')
			(*i)++;
		else if ((tmp[*i] == '\"') || (tmp[*i] == '\''))
			*i = quotes(tmp, *i);
		(*i)++;
	}
	tok = malloc(sizeof(char) * (*i - end));
	tok = ft_substr(tmp, end, *i - end);
	end = *i;
	return (tok);
}

int			parser(t_param *all, char **buf)
{
	char	*tmp;
	char	*tok;
	int		i;
	t_list	*vasya;

	vasya = NULL;
	i = 0;
	tmp = ft_strtrim(*buf, " \t\n");
	all->buf_len = ft_strlen(tmp);
	all->i = lexer(tmp);
	if (all->i == -1)
	{
		all->buf_len = 0;
		return (-1);
	}
	while (tmp[i])
	{
		tok = token_init(tmp, &i);
		while (tmp[i] == ' ' || tmp[i] == '\t')
			i++;
		ft_lstadd_back(&vasya, ft_lstnew(tok));
	}
	// printing
	while (vasya)
	{
		printf("%s\n", vasya->content);
		vasya = vasya->next;
	}

	if (all->buf_len > 0 && all->flag != -1)
	{
		all->cmd = ft_split(tmp, ' ');
		if (!all->cmd)
			ft_putendl("ERROR in process of split");
	}
	free(tmp);
	return (0);
}
