/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:55:10 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/11 18:55:56 by arannara         ###   ########.fr       */
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

char		*str_joiner(char *tmp3, char *tok, int *i, int *z)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_substr(tok, *z, *i - *z);
	tmp2 = ft_strjoin(tmp3, tmp);
	free(tmp);
	free(tmp3);

	return (tmp2);
}

char		*quote_remover(int *i, char *tok)
{
	char	c;
	char	*tmp3;
	int		z;

	c = tok[*i];
	(*i)++;
	z = *i;
	tmp3 = ft_calloc(sizeof(char), 2);
	while (tok[*i] && tok[*i] != c)
	{
		if (tok[*i] == '\\' && tok[*i + 1] != '\0' && c == '\"')
		{
			tmp3 = str_joiner(tmp3, tok, i, &z);
			(*i)++;
			z = *i;

		}
		// else if (tok[*i] == '\"' && tok[*i + 1] != '\0' && c == '\\')
		// {
		// 	tmp3 = str_joiner(tmp3, tok, i, &z);
		// 	(*i)++;
		// 	z = *i;
		// }
		(*i)++;
	}
	if (z != *i)
		tmp3 = str_joiner(tmp3, tok, i, &z);
	(*i)++;
	return (tmp3);
}

char		*token_handler(char *tok)
{
	char	*str;
	char	*tmp;
	char	*tmp2;
	int		i;
	int		z;

	i = 0;
	z = 0;
	str = ft_calloc(sizeof(char), 3);

	while (tok[i])
	{
		if (tok[i] == '\\' && tok[i + 1] == '\\')
		{
			tok = ft_strdup("\\");
			str[i] = tok[i];
			free(tok);
			return(str);
		}
		else if (tok[i] == '\\' && tok[i + 1] == '\"')
		{
			tok = ft_strdup("\"");
			str[i] = tok[i];
			free(tok);
			return(str);
		}
		else if (tok[i] == '$' && tok[i + 1] == '?')
		{
			free(str);
			return(ft_itoa(g_exit_code));
		}
		else if (tok[i] == '>' || tok[i] == '<' || tok[i] == ';' || tok[i] == '|')
		{
			while (tok[i])
			{
				str[i] = tok[i];
				i++;
			}
			return (str);
		}
		else
		{
			if (tok[i] == '\'' || tok[i] == '\"' || tok[i] == '\\')
			{

				str = str_joiner(str, tok, &i, &z);
				z = i;
				tmp = quote_remover(&i, tok);

				tmp2 = ft_strjoin(str, tmp);
				free(str);
				free(tmp);
				str = tmp2;

				z = i;
			}
			else
				i++;
		}
	}
	if (z != i)
		str = str_joiner(str, tok, &i, &z);
	return (str);
}

int			parser(t_param *all, char **buf)
{
	char	*tmp;
	char	**str;
	char	*str2;
	int		i;

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
	i = 0;
	while (str[i])
	{
		str2 = token_handler(str[i]);
		free(str[i]);
		str[i] = ft_strdup(str2);
		free(str2);
		i++;
	}
	ft_lstclear(&all->tmp_vasya, free);
	all->cmd = str;
	if (!all->cmd)
		ft_putendl("ERROR in process of parser");
	free(tmp);
	return (0);
}
