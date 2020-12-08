/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:18:51 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/15 15:12:22 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	char	stp;
	char	*tmp_d;
	char	*tmp_s;

	stp = (char)c;
	tmp_d = (char *)dest;
	tmp_s = (char *)src;
	while (n--)
	{
		*tmp_d++ = *tmp_s++;
		if (*(tmp_d - 1) == stp)
			return (tmp_d);
	}
	return (NULL);
}
