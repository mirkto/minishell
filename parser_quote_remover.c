/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quote_remover.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 19:17:55 by arannara          #+#    #+#             */
/*   Updated: 2021/01/25 18:03:43 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char		*exitcode_handler(int *z, int *i, t_param *all, char *tok)
{
	char	*tmp3;
	char	*tmp4;

	tmp3 = ft_calloc(sizeof(char), 2);
	if (*z != *i)
		tmp3 = str_joiner(tmp3, tok, i, z);
	(*i) += 2;
	tmp4 = ft_substr(tmp3, 0, ft_strlen(tmp3));
	all->tmp = ft_itoa(g_exit_code);
	free(tmp3);
	tmp3 = ft_strjoin(tmp4, all->tmp);
	free(tmp4);
	free(all->tmp);
	*z = *i;
	return (tmp3);
}

char		*dollar_in_quote_handler(int *z, int *i, t_param *all, char *tok)
{
	char	*tmp3;
	char	*tmp4;

	tmp3 = ft_calloc(sizeof(char), 2);
	if (*z != *i)
		tmp3 = str_joiner(tmp3, tok, i, z);
	*z = ++(*i);
	while (tok[*i] != '$' && tok[*i] != ' '
			&& tok[*i] != '\\' && tok[*i] != '\"'
			&& tok[*i] != ';' && tok[*i] != '|')
		(*i)++;
	tmp4 = ft_substr(tok, *z, (*i) - (*z));
	if (all->flag == 3)
		all->tmp = ft_strjoin("$", tmp4);
	else
		all->tmp = get_value_env(all, tmp4);
	free(tmp4);
	tmp4 = ft_strjoin(tmp3, all->tmp);
	free(tmp3);
	free(all->tmp);
	tmp3 = tmp4;
	*z = *i;
	return (tmp3);
}

char		*quote_remover2(int *i, char *tok, t_param *all, char *tmp3)
{
	if (tok[*i] == '\\' && tok[*i + 1] == '$' && all->c == '\"')
	{
		tmp3 = str_joiner(tmp3, tok, i, &all->z);
		(*i) += 1;
		all->z = *i;
		all->flag = 3;
	}
	else if (tok[*i] == '$' && tok[*i + 1] == '?' && all->c == '\"')
	{
		free(tmp3);
		tmp3 = exitcode_handler(&all->z, i, all, tok);
	}
	else if (tok[*i] == '$' && all->c == '\"')
	{
		all->qr2_tmp = ft_strdup(tmp3);
		all->qr2_tmp2 = dollar_in_quote_handler(&all->z, i, all, tok);
		free(tmp3);
		tmp3 = ft_strjoin(all->qr2_tmp, all->qr2_tmp2);
		free(all->qr2_tmp);
		free(all->qr2_tmp2);
	}
	else
		(*i)++;
	return (tmp3);
}

char		*quote_remover(int *i, char *tok, t_param *all)
{
	char	*tmp3;

	all->c = tok[(*i)++];
	all->z = *i;
	tmp3 = ft_calloc(sizeof(char), 2);
	while (tok[*i] && tok[*i] != all->c)
		tmp3 = quote_remover2(i, tok, all, tmp3);
	if (all->z != (*i))
		tmp3 = str_joiner(tmp3, tok, i, &all->z);
	(*i)++;
	return (tmp3);
}
