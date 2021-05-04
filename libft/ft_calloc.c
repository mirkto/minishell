#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*tmp;

	tmp = (void *)malloc(size * nmemb);
	if (!tmp)
		return (NULL);
	ft_bzero(tmp, size * nmemb);
	return (tmp);
}
