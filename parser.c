/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arannara <arannara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:55:10 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/28 17:02:15 by arannara         ###   ########.fr       */
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

char		*token_init(char *tmp, int *i)
{
	char	*tok;
	int		end;

	end = *i;
	while (tmp[*i] != '\0' && tmp[*i] != ' ' && tmp[*i] != '\t')
	{
		if (tmp[*i] == '\\' && tmp[*i + 1] != '\0')
			(*i)++;
		else if ((tmp[*i] == '\"') || (tmp[*i] == '\''))
			*i = quotes(tmp, *i);
		(*i)++;
	}
	// tok = malloc(sizeof(char) * (*i - end));
	tok = ft_substr(tmp, end, *i - end);
	end = *i;
	return (tok);
}

int			parser(t_param *all, char **buf)
{
	char	*tmp;
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

	t_list	*vasya;
	vasya = NULL;
	char	*tok;
	tok = NULL;
	i = 0;
	while (tmp[i])
	{
		tok = token_init(tmp, &i);
		while (tmp[i] == ' ' || tmp[i] == '\t')
			i++;
		ft_lstadd_back(&vasya, ft_lstnew(tok));
		all->num_of_toks++;
		// free(tok);
	}
	// // printing
	// while (vasya)
	// {
	// 	printf("%s\n", vasya->content);
	// 	vasya = vasya->next;
	// }
	// ft_putnbr(all->num_of_toks);

	char **str = (char **)malloc(sizeof(char *) * (all->num_of_toks + 1));
	if (!str)
		return (0);

	t_list	*tmp_vasya;
	tmp_vasya = vasya;
	i = 0;
	while (vasya)
	{
		str[i] = ft_strdup(vasya->content);
		vasya = vasya->next;
		i++;
	}
	str[i] = NULL;

	ft_lstclear(&tmp_vasya, free);

	all->cmd = str;
	if (!all->cmd)
		ft_putendl("ERROR in process of split");
	free(tmp);
	return (0);
}
