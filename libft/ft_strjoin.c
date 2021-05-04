#include "libft.h"

char		*ft_strjoin(const char *s1, const char *s2)
{
	char	*tmp;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	tmp = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		tmp[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		tmp[i++] = s2[j++];
	tmp[i] = '\0';
	return (tmp);
}

char		*ft_strstrjoin(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	int		len;
	int		i;
	int		j;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	if (!(tmp = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	while (s1[i])
	{
		tmp[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		tmp[i++] = s2[j++];
	j = 0;
	while (s3[j])
		tmp[i++] = s3[j++];
	tmp[i] = '\0';
	return (tmp);
}
