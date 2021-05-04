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
