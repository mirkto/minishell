/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:55:10 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/31 00:02:49 by arannara         ###   ########.fr       */
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
			if (end == *i && tmp[*i] == '>' && tmp[*i + 1] == '>')
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

char		**list_maker(t_param *all, char *tmp)
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


char *quote_remover(int *i, char *tok)
{
	char c;
	char *tmp;
	char *tmp2;
	char *tmp3;

	c = tok[*i];
	tmp = ft_substr(tok, 0, *i);
	(*i)++;
	while (tok[*i] && tok[*i] != c)
	{
		// if (tok[*i] == '\\' && tok[*i + 1] != '\0' && c == '\"')
		//  	(*i)++;
		(*i)++;
	}
	(*i)--;
	tmp2 = ft_substr(tok, ft_strlen(tmp) + 1, *i);
	tmp3 = ft_strjoin(tmp, tmp2);
	free(tmp);
	free(tmp2);
	return (tmp3);
}q

char *token_handler(char *tok)
{
	int i;

	i = 0;
	while (tok[i])
	{
		if (tok[i] == '>' || tok[i] == '<' || tok[i] == ';' || tok[i] == '|')
			return (tok);
		else
		{
			if (tok[i] == '\'' || tok[i] == '\"' )
				tok = quote_remover(&i, tok);
			// else if (tok[i][j] == '\\')
			// {
			// }
			// else if (tok[i][j] == '$')
			// {
			// }
			else
				i++;
		}
	}
	return (tok);
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
	str = list_maker(all, tmp);

	int i = 0;
	while (str[i])
	{
		str[i] = token_handler(str[i]);
		i++;
	}

	ft_lstclear(&all->tmp_vasya, free);
	all->cmd = str;
	if (!all->cmd)
		ft_putendl("ERROR in process of split");
	free(tmp);
	return (0);
}
