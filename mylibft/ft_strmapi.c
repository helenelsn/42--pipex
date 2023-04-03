/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 18:28:34 by hlesny            #+#    #+#             */
/*   Updated: 2022/11/13 01:47:03 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*fstr;
	unsigned int	i;

	i = 0;
	fstr = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!fstr)
		return (0);
	while (s[i])
	{
		fstr[i] = (*f)(i, s[i]);
		i++;
	}
	fstr[i] = '\0';
	return (fstr);
}
