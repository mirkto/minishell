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

char		*join_str_tmp(t_param *all, char *str, char *tmp, int i)
{
	char	*tmp2;

	tmp2 = ft_strjoin(str, tmp);
	free(str);
	free(tmp);
	all->z = i;
	return (tmp2);
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
	all->i = 0;
	all->z = 0;
	all->p_str = ft_calloc(sizeof(char), 3);
	while (tok[all->i])
	{
		if (tok[all->i] == '\\')
			slash_processing(all, tok);
		else if (tok[all->i] == '>' || tok[all->i] == '<'
				|| tok[all->i] == ';' || tok[all->i] == '|')
		{
			all->i--;
			while (tok[++all->i])
				all->p_str[all->i] = tok[all->i];
			return (all->p_str);
		}
		else
		{
			if (quote_processing(all, tok) == 1)
				return (dollar_handler(all, tok, &all->i, all->p_str));
		}
	}
	if (all->z != all->i)
		all->p_str = str_joiner(all->p_str, tok, &all->i, &all->z);
	return (all->p_str);
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
	all->j = -1;
	while (str[++all->j])
	{
		str2 = token_handler(all, str[all->j]);
		free(str[all->j]);
		str[all->j] = ft_strdup(str2);
		free(str2);
	}
	ft_lstclear(&all->tmp_vasya, free);
	all->cmd = str;
	free(tmp);
	free(all->buf);
	return (0);
}
