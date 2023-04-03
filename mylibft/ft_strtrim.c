/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 18:02:52 by hlesny            #+#    #+#             */
/*   Updated: 2022/11/13 01:54:13 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isin(const char *s, int c)
{
	size_t	i;

	i = 0;
	if (c == '\0')
		return (1);
	while (s[i])
	{
		if (s[i] == (char)c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = ft_strlen(s1);
	if (!set)
		return ((char *)s1);
	while (s1[i] && ft_isin(set, s1[i]))
		i++;
	if (!s1[i])
		return (ft_strdup(""));
	while (ft_isin(set, s1[--j]))
		;
	return (ft_substr(s1, i, j - i + 1));
}
