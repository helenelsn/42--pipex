/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 19:53:01 by hlesny            #+#    #+#             */
/*   Updated: 2022/11/13 01:52:25 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;
	size_t	offset;
	size_t	src_index;

	s_len = ft_strlen(src);
	if (!dst && size == 0)
		return (s_len);
	d_len = ft_strlen(dst);
	offset = d_len;
	src_index = 0;
	if (size == 0 || size < d_len)
		return (size + s_len);
	while (*(src + src_index) && offset < size - 1)
	{
		*(dst + offset) = *(src + src_index);
		offset++;
		src_index++;
	}
	*(dst + offset) = '\0';
	return (d_len + s_len);
}
