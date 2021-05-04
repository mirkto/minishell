#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	char	stp;
	char	*tmp_d;
	char	*tmp_s;

	stp = (char)c;
	tmp_d = (char *)dest;
	tmp_s = (char *)src;
	while (n--)
	{
		*tmp_d++ = *tmp_s++;
		if (*(tmp_d - 1) == stp)
			return (tmp_d);
	}
	return (NULL);
}
