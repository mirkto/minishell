/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:18:14 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/19 10:35:21 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_itoa(int n)
{
	int		i;
	int		len_nbr;
	char	*tmp;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	len_nbr = ft_nbrlen(n);
	tmp = (char *)malloc(sizeof(char) * (len_nbr + 1));
	if (!tmp)
		return (NULL);
	tmp[len_nbr] = '\0';
	i = 0;
	if (n < 0)
	{
		tmp[0] = '-';
		n *= -1;
		i++;
	}
	while (i < len_nbr--)
	{
		tmp[len_nbr] = (n % 10) + '0';
		n /= 10;
	}
	return (tmp);
}
