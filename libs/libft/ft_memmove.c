/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:21:01 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/15 21:46:55 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*tmp_d;
	char	*tmp_s;

	tmp_d = (char *)dest;
	tmp_s = (char *)src;
	if (!tmp_d && !tmp_s)
		return (dest);
	if (tmp_d < tmp_s)
		while (n--)
			*tmp_d++ = *tmp_s++;
	else
		while (n--)
			tmp_d[n] = tmp_s[n];
	return (dest);
}
