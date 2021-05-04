#include "minishell.h"

int			is_quote_close(int *i, char *tmp)
{
	char c;

	c = tmp[*i];
	(*i)++;
	while (tmp[*i] != c)
	{
		if (tmp[*i] == '\\' && c == '\"' && tmp[*i + 1] != '\0')
			(*i)++;
		else if (tmp[*i] == '\0')
			return (FALSE);
		(*i)++;
	}
	if (tmp[(*i)] == c)
	{
		(*i)++;
		return (TRUE);
	}
	return (FALSE);
}

int			lexer3(char *tmp)
{
	int		i;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\\')
			i++;
		else if (tmp[i] == '\'' || tmp[i] == '\"')
		{
			if (is_quote_close(&i, tmp) == TRUE)
				continue ;
			else
				return (put_error("quotes are not close", 0));
		}
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
		if (tmp[i] == '|' || tmp[i] == ';')
		{
			i++;
			while (tmp[i] == ' ' || tmp[i] == '\t')
				i++;
			if (tmp[i] == '|' || tmp[i] == ';')
				return (put_error("syntax error near unexpected", 0));
		}
		i++;
	}
	return (lexer2(tmp));
}
