/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 04:34:20 by hlesny            #+#    #+#             */
/*   Updated: 2023/04/07 20:46:01 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest [i] = '\0';
		i++;
	}
	return (dest);
}

static int	count_words(const char *str, char charset)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == charset))
			i++;
		if (str[i] && !(str[i] == charset))
		{
			count++;
			while (str[i] && !(str[i] == charset))
				i++;
		}
	}
	return (count);
}

static void	free_tab(char ***s, int i)
{
	while (i-- > 0)
		free((*s)[i]);
	free(*s);
	*s = NULL;
}

static void	fill_words(char **split, const char *str, char charset, int words)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (i < words)
	{
		while (str[j] && str[j] == charset)
			j++;
		k = 0;
		while (str[j + k] && !(str[j + k] == charset))
			k++;
		split[i] = ft_calloc((k + 1), sizeof(char));
		if (!split[i])
			return (free_tab(&split, i));
		ft_strncpy(split[i], &str[j], k);
		j += k;
		i++;
	}
	split[i] = 0;
}

char	**ft_split(const char *str, char charset)
{
	char	**split;
	int		words_c;

	if (str == NULL)
		return (NULL);
	words_c = (count_words(str, charset));
	split = ft_calloc((words_c + 1), sizeof(char *));
	if (split == NULL)
		return (NULL);
	fill_words(split, str, charset, words_c);
	return (split);
}
