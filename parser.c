/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 16:55:10 by ngonzo            #+#    #+#             */
/*   Updated: 2020/12/02 16:55:14 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_buf_lst(t_param *all)
{
	all->i = 0;
	while (all->buf_lst[all->i])
	{
		free(all->buf_lst[all->i]);
		all->i++;
	}
	free(all->buf_lst);
}

int		check_comma(char *tmp, t_param *all)
{
	char	num_c;
	int		c1;
	int		c2;

	num_c = 0;
	all->i = 0;
	c1 = 0;
	c2 = 0;
	all->commas = 0;
	while (all->i < all->buf_len)
	{
		if (tmp[all->i] == '\'')
			c1++;
		if (tmp[all->i] == '\"')
			c2++;
		all->i++;
	}
	if ((c1 % 2) != 0 || (c2 % 2) != 0)
		all->commas = -1;
	return (0);
}

int		parser(t_param *all, char **buf)
{
	char	*tmp;

	tmp = ft_strtrim(*buf, "\v\f\r \n\t");
	all->buf_len = ft_strlen(tmp);
	check_comma(tmp, all);
	if (all->buf_len > 0 && all->commas != -1)
	{
		all->buf_lst = ft_split_commas(tmp, ' ');
		if (!all->buf_lst)
			ft_putendl("ERROR in process of split");
	}
	free(tmp);
	return (0);
}