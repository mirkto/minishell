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

#define PRINT_LEN_WORD ft_putnbr(len);
#define PRINT_AMOUNT_WORDS ft_putnbr(amount_words);
#define COMMAS_FNC tmp = s[i]; i++; while (s[i] != tmp) {i++;}
#define REDUCTION size_t num_w; size_t num_l; size_t ios; int len; char tmp;
#define REDUCTION_FNC RF1 RF2 RF3 RF4 RF5 RF6
#define RF1 while (s[num_w] == '"' || s[num_w] == '\'') {
#define RF2 	tmp = s[num_w];
#define RF3 	str[ios][num_l++] = s[num_w++];
#define RF4 	while (s[num_w] != tmp)
#define RF5 		str[ios][num_l++] = s[num_w++];
#define RF6 	str[ios][num_l++] = s[num_w++]; }

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
		if (s[i] == '"' || s[i] == '\'')
		{
			COMMAS_FNC;
		}
		if ((s[i] == '"' && s[i + 1] != c && s[i + 1] != '\0') ||
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

	i = -1;
	sp = 0;
	while (s[++i] == c)
		sp++;
	if (s[i] == '"' || s[i] == '\'')
	{
		while (s[i] == '"' || s[i] == '\'')
		{
			tmp = s[i];
			i++;
			while (s[i] != tmp)
				i++;
			i++;
		}
	}
	else
	{
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (i - sp);
}

char	**write_words(char **str, const char *s, char c, size_t count_words)
{
	REDUCTION;
	num_w = 0;
	ios = 0;
	while (ios < count_words)
	{
		len = len_word(&s[num_w], c);
		// PRINT_LEN_WORD;
		str[ios] = (char *)malloc(sizeof(char) * (len + 1));
		if (!str || len == -1)
			return (free_split(str, ios - 1));
		while (s[num_w] == c)
			num_w++;
		num_l = 0;
		if (s[num_w] == '"' || s[num_w] == '\'')
		{
			REDUCTION_FNC;
		}
		else
			while (s[num_w] != c && s[num_w] != '\0')
				str[ios][num_l++] = s[num_w++];
		str[ios][num_l] = '\0';
		ios++;
	}
	str[ios] = NULL;
	return (str);
}

char	**ft_split_commas(const char *s, char c)
{
	char		**str;
	int			amount_words;

	if (!s)
		return (NULL);
	amount_words = count_words(s, c);
	// PRINT_AMOUNT_WORDS;
	if (amount_words == -1)
		return (NULL);
	str = (char **)malloc(sizeof(char *) * (amount_words + 1));
	if (!str)
		return (NULL);
	str = write_words(str, s, c, amount_words);
	return (str);
}
