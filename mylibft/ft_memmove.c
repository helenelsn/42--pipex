/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 22:02:16 by hlesny            #+#    #+#             */
/*   Updated: 2022/11/12 23:38:23 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*destination;
	char	*source;

	destination = dest;
	source = (char *)src;
	i = 0;
	if (destination < source)
	{
		while (i < n && (destination || source))
		{
			destination[i] = source[i];
			i++;
		}
	}
	else
	{
		while (n-- > 0 && (destination || source))
		destination[n] = source [n];
	}
	return (dest);
}
