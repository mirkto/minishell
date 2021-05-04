#include "libft.h"

char		*ft_strdup(const char *str)
{
	int		i;
	char	*tmp;

	tmp = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (str[i])
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}
