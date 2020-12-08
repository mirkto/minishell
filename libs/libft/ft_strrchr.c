/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:25:11 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/15 18:20:00 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		len;
	char	*tmp;

	len = ft_strlen(s);
	tmp = (char *)&s[0];
	tmp += len;
	while (len >= 0)
	{
		if (*tmp == (char)c)
			return (tmp);
		tmp--;
		len--;
	}
	return (NULL);
}
