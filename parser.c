/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:55:10 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/25 20:44:03 by arannara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int		check_comma(char *tmp, t_param *all)
// {
// 	char	num_c;
// 	int		c1;
// 	int		c2;

// 	num_c = 0;
// 	all->i = 0;
// 	c1 = 0;
// 	c2 = 0;
// 	all->flag = 0;
// 	while (all->i < all->buf_len)
// 	{
// 		if (tmp[all->i] == '\'')
// 			c1++;
// 		if (tmp[all->i] == '"')
// 			c2++;
// 		all->i++;
// 	}
// 	if ((c1 % 2) != 0 || (c2 % 2) != 0)
// 		all->flag = -1;
// 	return (0);
// }

int	lexer3(char *tmp)
{
	int i;

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

int	lexer2(char *tmp)
{
	int i;

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
			return (put_error("syntax error near unexpected token `newline'", 0));
		if ((tmp[i] == '>' && tmp[i + 1] == '>' ) && (tmp[i + 2]) == '\0')
			return (put_error("syntax error near unexpected token `newline'", 0));
		i++;
	}
	return (lexer3(tmp));
}

int	lexer(char *tmp)
{
	int i; // obrabotka ";;" "cmd ;" "|" ";"

	if ((tmp[0]) == '|' || tmp[0] == ';')
		return (put_error("syntax error near unexpected token `;' or `|'", 0));
	i = 0;
	while (tmp[i])
	{
		/* it is don't error */
		if (tmp[i] == ';' && (tmp[i + 1]) == '\0')
			return (put_error("syntax ERROR", 0));
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
				return (put_error("syntax error near unexpected token `;;'", 0));
		}
	return (lexer2(tmp));
}

// char **token_init(char *tmp)
// {
// 	int i;

// 	i = 0;
// 	while ()
// 	{


// 	}

// }

int		parser(t_param *all, char **buf)
{
	char	*tmp;

	tmp = ft_strtrim(*buf, " \t\n");
	all->buf_len = ft_strlen(tmp);
	// check_comma(tmp, all);
	all->i = lexer(tmp);
	if (all->i == -1)
	{
		all->buf_len = 0;
		return (-1);
	}

	// token_init(tmp);

	if (all->buf_len > 0 && all->flag != -1)
	{
		all->cmd = ft_split(tmp, ' ');
		if (!all->cmd)
			ft_putendl("ERROR in process of split");
	}
	free(tmp);
	return (0);
}
