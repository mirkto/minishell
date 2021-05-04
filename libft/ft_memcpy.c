#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*tmp_d;
	char	*tmp_s;

	tmp_d = (char *)dest;
	tmp_s = (char *)src;
	if (!tmp_d && !tmp_s)
		return (dest);
	while (n--)
		*tmp_d++ = *tmp_s++;
	return (dest);
}
