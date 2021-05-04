#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*tmp;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
	{
		tmp = (char *)malloc(sizeof(char));
		if (!tmp)
			return (NULL);
		*tmp = '\0';
		return (tmp);
	}
	tmp = (char *)malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < len)
	{
		tmp[i] = s[start + i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}
