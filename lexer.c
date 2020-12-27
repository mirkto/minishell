/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 19:32:49 by arannara          #+#    #+#             */
/*   Updated: 2020/12/27 19:34:37 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			lexer3(char *tmp)
{
	int		i;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '<' && (tmp[i + 1]) == '|')
			return (put_error("syntax error near unexpected token `|'", 0));
		else if (tmp[i] == '|' && (tmp[i + 1]) == '|')
			return (put_error("syntax error near unexpected token `||'", 0));
		else if (tmp[i] == '&' && (tmp[i + 1]) == '&')
			return (put_error("syntax error near unexpected token `&&'", 0));
		i++;
	}
	return (0);
}

int			lexer2(char *tmp)
{
	int		i;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == ';' && (tmp[i + 1]) == '|')
			return (put_error("syntax error near unexpected token `|'", 0));
		else if (tmp[i] == '|' && (tmp[i + 1]) == ';')
			return (put_error("syntax error near unexpected token `;'", 0));
		i++;
	}
	i = 0;
	while (tmp[i])
	{
		if ((tmp[i] == '<' || tmp[i] == '>') && (tmp[i + 1]) == '\0')
			return (put_error("syntax error near unexpected token", 0));
		if ((tmp[i] == '>' && tmp[i + 1] == '>') && (tmp[i + 2]) == '\0')
			return (put_error("syntax error near unexpected token", 0));
		i++;
	}
	return (lexer3(tmp));
}

int			lexer(char *tmp)
{
	int		i;

	if ((tmp[0]) == '|' || tmp[0] == ';')
		return (put_error("syntax error near unexpected token `;' or `|'", 0));
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == ';' && (tmp[i + 1]) == ';')
			return (put_error("syntax error near unexpected token `;;'", 0));
		i++;
	}
	i = 0;
	while (tmp[i++])
		if (tmp[i++] == ';')
		{
			while (tmp[i] == ' ' || tmp[i] == '\t')
				i++;
			if (tmp[i] == ';')
				return (put_error("syntax error near unexpected token ;", 0));
		}
	return (lexer2(tmp));
}
