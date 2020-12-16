/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngonzo <ngonzo@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 19:05:04 by ngonzo            #+#    #+#             */
/*   Updated: 2020/05/15 15:34:49 by ngonzo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*first_new;
	t_list	*cur_new;
	t_list	*first_old;
	t_list	*cur_old;

	if (!lst)
		return (NULL);
	first_old = lst;
	first_new = ft_lstnew(f(first_old->content));
	if (!first_new)
		return (NULL);
	cur_new = first_new;
	cur_old = first_old->next;
	while (cur_old != NULL)
	{
		cur_new->next = ft_lstnew(f(cur_old->content));
		if (!cur_new->next)
		{
			ft_lstclear(&first_new, del);
			return (NULL);
		}
		cur_new = cur_new->next;
		cur_old = cur_old->next;
	}
	return (first_new);
}
