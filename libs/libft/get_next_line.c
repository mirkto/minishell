/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 20:50:02 by ngonzo            #+#    #+#             */
/*   Updated: 2020/09/25 16:27:46 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define BUFFER_SIZE 42

static	void	free_buf(char *buf)
{
	free(buf);
	buf = NULL;
}

static	char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*newstr;
	char	*ptr;
	size_t	i;
	size_t	j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	if (!(newstr = (char*)malloc((i + j + 1) * sizeof(char))))
		return (NULL);
	ptr = newstr;
	while (*s1)
		*newstr++ = *s1++;
	free(s1 - i);
	s1 = NULL;
	while (*s2)
		*newstr++ = *s2++;
	*newstr = '\0';
	return (ptr);
}

static	int		result_with_n(char **line, char **archive, char *n_ptr)
{
	char	*temp;

	*n_ptr = '\0';
	*line = ft_strdup(*archive);
	n_ptr++;
	temp = ft_strdup(n_ptr);
	free(*archive);
	*archive = temp;
	return (1);
}

static	int		result_without_n(char **line, char **archive, int ret)
{
	char	*n_ptr;

	if (ret == -1)
		return (-1);
	if (*archive && (n_ptr = ft_strchr(*archive, '\n')))
		return (result_with_n(line, archive, n_ptr));
	if (*archive)
	{
		*line = *archive;
		*archive = NULL;
		return (0);
	}
	*line = ft_strdup("");
	return (0);
}

int				get_next_line(int fd, char **line)
{
	int			ret;
	char		*buf;
	static char	*archive;
	char		*n_ptr;

	if (BUFFER_SIZE < 1 || fd < 0 || !line || read(fd, archive, 0) != 0 ||
		!(buf = (char*)malloc((BUFFER_SIZE + 1) * sizeof(char))))
		return (-1);
	while ((ret = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[ret] = '\0';
		archive = ft_strjoin_free(archive, buf);
		if ((n_ptr = ft_strchr(archive, '\n')))
		{
			free_buf(buf);
			return (result_with_n(line, &archive, n_ptr));
		}
	}
	free_buf(buf);
	return (result_without_n(line, &archive, ret));
}
