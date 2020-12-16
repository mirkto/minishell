/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:25:46 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/14 17:10:05 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_find(const char *set, char s)
{
	int		i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == s)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	i;
	size_t	max_len;
	size_t	start;
	char	*tmp;

	if (!s1)
		return (NULL);
	max_len = ft_strlen(s1);
	while (max_len > 0 && ft_find(set, s1[max_len - 1]))
		max_len--;
	start = 0;
	while (start < max_len && ft_find(set, s1[start]))
		start++;
	tmp = (char *)malloc(sizeof(char) * (max_len - start + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < max_len - start)
	{
		tmp[i] = s1[start + i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}
