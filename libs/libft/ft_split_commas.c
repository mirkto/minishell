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

void	*free_split(char **s, int num_w)
{
	while (num_w >= 0)
	{
		free(s[num_w]);
		num_w--;
	}
	free(s);
	return (NULL);
}

int		count_words(const char *s, char c)
{
	size_t		i;
	size_t		count;
	char		tmp;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
			count++;
		if (s[i] == '\"' || s[i] == '\'')
		{
			tmp = s[i];
			i++;
			while (s[i] != tmp)
				i++;
		}
		if ((s[i] == '\"' && s[i + 1] != c && s[i + 1] != '\0') ||
			(s[i] == '\'' && s[i + 1] != c && s[i + 1] != '\0'))
			count--;
		else
		{
			while (s[i] != c && s[i + 1] != '\0')
				i++;
		}
		i++;
	}
	return (count);
}

size_t	len_word(const char *s, char c)
{
	size_t		i;
	size_t		sp;
	char		tmp;

	i = 0;
	sp = 0;
	while (s[i] == c)
	{
		i++;
		sp++;
	}
	if (s[i] == '\"' || s[i] == '\'')
	{
		tmp = s[i];
		i++;
		while (s[i] != tmp)
			i++;
		i++;
	}
	else
	{
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (i - sp);
}

void	utils_for_write_words(char **str, const char *s, size_t n[3])
{
	char		tmp;

	tmp = s[n[0]];
	str[n[1]][n[2]++] = s[n[0]++];
	while (s[n[0]] != tmp)
		str[n[1]][n[2]++] = s[n[0]++];
	str[n[1]][n[2]++] = s[n[0]++];
}

char	**write_words(char **str, const char *s, char c, size_t count_words)
{
	size_t		n[3];
	int			len;

	n[0] = 0;
	n[1] = 0;
	while (n[1] < count_words)
	{
		len = len_word(&s[n[0]], c);
		// ft_putnbr(len);//--
		str[n[1]] = (char *)malloc(sizeof(char) * (len + 1));
		if (!str || len == -1)
			return (free_split(str, n[1] - 1));
		while (s[n[0]] == c)
			n[0]++;
		n[2] = 0;
		if (s[n[0]] == '\"' || s[n[0]] == '\'')
			utils_for_write_words(str, s, &n[0]);
		else
			while (s[n[0]] != c && s[n[0]] != '\0')
				str[n[1]][n[2]++] = s[n[0]++];
		str[n[1]][n[2]] = '\0';
		n[1]++;
	}
	str[n[1]] = NULL;
	return (str);
}

char	**ft_split_commas(const char *s, char c)
{
	char		**str;
	int			amount_words;

	if (!s)
		return (NULL);
	amount_words = count_words(s, c);
	// ft_putnbr(amount_words);//--
	if (amount_words == -1)
		return (NULL);
	str = (char **)malloc(sizeof(char *) * (amount_words + 1));
	if (!str)
		return (NULL);
	str = write_words(str, s, c, amount_words);
	return (str);
}
