/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 20:12:25 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/11 15:26:47 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*tmp;

	if (!lst || !f || !del)
		return (NULL);
	new = ft_lstnew(f(lst->content));
	if (!new)
		return (NULL);
	tmp = new;
	while (lst->next)
	{
		lst = lst->next;
		tmp->next = ft_lstnew(f(lst->content));
		if (!(tmp->next))
		{
			ft_lstclear(&new, del);
			return (NULL);
		}
		tmp = tmp->next;
	}
	return (new);
}
