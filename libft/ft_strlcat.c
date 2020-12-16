/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:23:46 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/19 09:33:41 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len_d;
	size_t	len_s;

	len_d = ft_strlen(dst);
	len_s = ft_strlen(src);
	if (len_d >= size || size == 0)
		return (len_s + size);
	if (len_s + 1 <= size - len_d - 1)
		ft_strlcpy(&dst[len_d], src, len_s + 1);
	else
		ft_strlcpy(&dst[len_d], src, size);
	dst[size - 1] = '\0';
	return (len_s + len_d);
}
