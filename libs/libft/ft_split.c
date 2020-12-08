/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:22:02 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/19 10:36:10 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_split(char **s, int num_w)
{
	while (num_w >= 0)
	{
		free(s[num_w]);
		num_w--;
	}
	free(s);
}

size_t	ft_count_words(const char *s, char c)
{
	size_t		i;
	size_t		count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
			count++;
		while (s[i] != c && s[i + 1] != '\0')
			i++;
		i++;
	}
	return (count);
}

size_t	ft_len_word(const char *s, char c)
{
	size_t		i;
	size_t		len;

	i = 0;
	len = 0;
	while (s[i] == c)
		i++;
	while (s[i] != c && s[i] != '\0')
	{
		len++;
		i++;
	}
	return (len);
}

char	**ft_write_words(char **str, const char *s, char c, size_t count_words)
{
	size_t		num_w;
	size_t		num_l;
	size_t		ind_of_s;

	num_w = 0;
	ind_of_s = 0;
	while (num_w < count_words)
	{
		str[num_w] = (char *)malloc(sizeof(char) * \
											(ft_len_word(&s[ind_of_s], c) + 1));
		if (!str)
		{
			ft_free_split(str, num_w - 1);
			return (NULL);
		}
		while (s[ind_of_s] == c)
			ind_of_s++;
		num_l = 0;
		while (s[ind_of_s] != c && s[ind_of_s] != '\0')
			str[num_w][num_l++] = s[ind_of_s++];
		str[num_w][num_l] = '\0';
		num_w++;
	}
	str[num_w] = 0;
	return (str);
}

char	**ft_split(const char *s, char c)
{
	char		**str;
	size_t		count_words;

	if (!s)
		return (NULL);
	count_words = ft_count_words(s, c);
	str = (char **)malloc(sizeof(char *) * (count_words + 1));
	if (!str)
		return (NULL);
	str = ft_write_words(str, s, c, count_words);
	return (str);
}
