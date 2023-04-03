/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 17:54:20 by Helene            #+#    #+#             */
/*   Updated: 2022/11/12 04:10:05 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*lstfirst;
	t_list	*current;

	if (lst)
	{
		lstfirst = ft_lstnew(f(lst->content));
		if (!lstfirst)
			return (lstfirst);
		lst = lst->next;
	}
	while (lst)
	{
		current = ft_lstnew(f(lst->content));
		if (!current)
		{
			ft_lstclear(&lstfirst, del);
			return (0);
		}
		ft_lstadd_back(&lstfirst, current);
		lst = lst->next;
	}
	return (lstfirst);
}
