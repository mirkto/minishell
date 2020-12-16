/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:19:32 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/15 15:07:17 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*tmp;
	unsigned char	t_c;

	tmp = (unsigned char *)s;
	t_c = (unsigned char)c;
	while (n--)
		if (*tmp++ == t_c)
			return (tmp - 1);
	return (NULL);
}
