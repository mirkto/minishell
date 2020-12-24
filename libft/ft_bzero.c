/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 09:20:06 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/15 14:10:18 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*tmp;
	size_t	i;

	tmp = s;
	i = 0;
	while (i < n)
	{
		tmp[i] = '\0';
		i++;
	}
}

void	ft_bzero_str(char *str)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(str);
	i = 0;
	while (i < len)
	{
		str[i] = '\0';
		i++;
	}
}