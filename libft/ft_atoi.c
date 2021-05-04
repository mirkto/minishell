#include "libft.h"

/*
**	int count = 0; and count++; in last while
**	and must unsigned long long result;
**	if (result > 9223372036854775807 || count > 19)
**		return ((sign == -1) ? 0 : -1);
*/

size_t	ft_lennbr(long int nbr)
{
	size_t		len;

	len = 0;
	if (nbr == 0)
		return (1);
	while (nbr != 0)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

int		ft_atoi(const char *nptr)
{
	int			i;
	long int	result;
	int			sign;

	i = 0;
	result = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = (result * 10) + (nptr[i] - '0');
		i++;
	}
	if (ft_lennbr(result) > 10 || result > 2147483648)
		return ((sign == -1) ? 0 : -1);
	return ((int)result * sign);
}
