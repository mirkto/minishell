/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quote_remover.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 19:17:55 by arannara          #+#    #+#             */
/*   Updated: 2021/01/17 20:04:06 by arannara         ###   ########.fr       */
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

void		exitcode_handler(int *z, int *i, t_param *all, char *tok)
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
}

void		dollar_in_quote_handler(int *z, int *i, t_param *all, char *tok)
{
	char	*tmp3;
	char	*tmp4;

	tmp3 = ft_calloc(sizeof(char), 2);
	if (*z != *i)
		tmp3 = str_joiner(tmp3, tok, i, z);
	*z = ++(*i);
	while (tok[*i] != '$' && tok[*i] != ' '
			&& tok[*i] != '\\' && tok[*i] != '\"')
		(*i)++;
	tmp4 = ft_substr(tok, *z, (*i) - (*z));
	all->tmp = get_value_env(all, tmp4);
	free(tmp4);
	tmp4 = ft_strjoin(tmp3, all->tmp);
	free(tmp3);
	free(all->tmp);
	tmp3 = tmp4;
	*z = *i;
}

char		*quote_remover(int *i, char *tok, t_param *all)
{
	char	c;
	int		z;
	char	*tmp3;

	c = tok[(*i)++];
	z = *i;
	tmp3 = ft_calloc(sizeof(char), 2);
	while (tok[*i] && tok[*i] != c)
	{
		if (tok[*i] == '\\' && tok[*i + 1] != '\0' && c == '\"')
		{
			tmp3 = str_joiner(tmp3, tok, i, &z);
			(*i) += 2;
			z = *i;
		}
		else if (tok[*i] == '$' && tok[*i + 1] == '?' && c == '\"')
			exitcode_handler(&z, i, all, tok);
		else if (tok[*i] == '$' && c == '\"')
			dollar_in_quote_handler(&z, i, all, tok);
		else
			(*i)++;
	}
	if (z != (*i)++)
		tmp3 = str_joiner(tmp3, tok, i, &z);
	return (tmp3);
}
