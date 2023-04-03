/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 18:07:55 by hlesny            #+#    #+#             */
/*   Updated: 2022/11/12 05:34:16 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*conc;
	size_t	i;
	size_t	size1;
	size_t	size2;

	i = 0;
	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	conc = malloc(sizeof(char) * (size1 + size2 + 1));
	if (!conc)
		return (0);
	while (i < size1 && s1[i])
	{
		conc[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < size2 && s2[i])
	{
		conc[size1 + i] = s2[i];
		i++;
	}
	conc[size1 + size2] = '\0';
	return (conc);
}
