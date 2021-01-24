/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 19:08:54 by arannara          #+#    #+#             */
/*   Updated: 2021/01/17 19:18:02 by arannara         ###   ########.fr       */
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

void		quote_processing(t_param *all, char *tok)
{
	if (all->z != all->i)
	{
		all->p_str = str_joiner(all->p_str, tok, &all->i, &all->z);
		all->z = all->i;
	}
	if (tok[all->i] == '\'' || tok[all->i] == '\"')
	{
		if (all->z != all->i)
		{
			all->p_str = str_joiner(all->p_str, tok, &all->i, &all->z);
			all->z = all->i;
		}
		all->tmp = quote_remover(&all->i, tok, all);
		all->p_str = join_str_and_tmp(all, all->p_str, all->tmp, all->i);
	}
	else
		all->i++;
}

void		slash_processing(t_param *all, char *tok)
{
	if (all->z != all->i)
	{
		all->p_str = str_joiner(all->p_str, tok, &all->i, &all->z);
		all->z = all->i;
	}
	if (tok[all->i] == '\\')
	{
		if (all->z != all->i)
		{
			all->p_str = str_joiner(all->p_str, tok, &all->i, &all->z);
			all->z = all->i;
		}
		all->tmp = slash_remover(&all->i, tok, all);
		all->p_str = join_str_and_tmp(all, all->p_str, all->tmp, all->i);
	}
	else
		all->i++;
}
