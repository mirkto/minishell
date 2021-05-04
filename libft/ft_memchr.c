#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*tmp;
	unsigned char	t_c;

	tmp = (unsigned char *)s;
	t_c = (unsigned char)c;
	while (n--)
		if (*tmp++ == t_c)
			return (tmp - 1);
	return (NULL);
}
