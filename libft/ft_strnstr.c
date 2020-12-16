/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:24:41 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/21 17:30:18 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	k;
	size_t	len_l;
	size_t	max_len;

	len_l = ft_strlen(little);
	if (!len_l)
		return ((char *)big);
	max_len = ft_strlen(big);
	if (max_len > len)
		max_len = len;
	i = 0;
	while (len_l + i <= max_len)
	{
		k = 0;
		while (big[i + k] == little[k])
		{
			if (k + 1 == len_l)
				return ((char *)big + i);
			k++;
		}
		i++;
	}
	return (NULL);
}
