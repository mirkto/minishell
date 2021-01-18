/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:55:10 by ngonzo            #+#    #+#             */
/*   Updated: 2021/01/18 19:11:16 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*token_handler(t_param *all, char *tok)
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
			if (z != i)
			{
				str = str_joiner(str, tok, &i, &z);
				z = i;
			}
			if (tok[i] == '\'' || tok[i] == '\"' )
			{
				str = str_joiner(str, tok, &i, &z);
				z = i;
				tmp = quote_remover(&i, tok, all);
				tmp2 = ft_strjoin(str, tmp);
				free(str);
				free(tmp);
				str = tmp2;
				z = i;
			}
			if (tok[i] == '$' && tok[i + 1] == '?')
			{
				tmp = str;
				str = ft_itoa(g_exit_code);
				tmp2 = ft_strjoin(tmp, str);
				free(str);
				free(tmp);
				str = tmp2;
				return(str);
			}
			else if (tok[i] == '$')
			{
				tmp = str;
				str = get_value_env(all, &tok[i + 1]);
				tmp2 = ft_strjoin(tmp, str);
				free(str);
				free(tmp);
				str = tmp2;
				return(str);
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
	// if (lexer(tmp) == -1)
	// {
	// 	free(tmp);
	// 	return (-1);
	// }
	str = list_maker(all, tmp);
	i = 0;
	while (str[i])
	{
		str2 = token_handler(all, str[i]);
		free(str[i]);
		str[i] = ft_strdup(str2);
		// str[i] = dollar_remover(all, str2);
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
