/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 17:18:26 by Helene            #+#    #+#             */
/*   Updated: 2022/11/12 04:09:36 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*elem;

	while (lst && *lst)
	{
		elem = *lst;
		*lst = ((*lst)->next);
		ft_lstdelone(elem, del);
	}
	lst = NULL;
}
