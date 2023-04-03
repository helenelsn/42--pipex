/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 01:58:05 by hlesny            #+#    #+#             */
/*   Updated: 2022/11/08 20:16:09 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	is_numeral(char c)
{
	return (c >= '0' && c <= '9');
}

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\n'
		|| c == '\t' || c == '\v'
		|| c == '\f' || c == '\r');
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	neg;
	int	number;

	i = 0;
	neg = 1;
	number = 0;
	while (nptr[i] && is_whitespace(nptr[i]))
		i++;
	if (nptr[i] && (nptr[i] == '-' || nptr[i] == '+'))
	{
		if (nptr[i] == '-')
			neg = -neg;
		i++;
	}
	while (nptr[i] && is_numeral(nptr[i]))
	{
		number = number * 10 + (nptr[i] - 48);
		i++;
	}
	return (neg * number);
}
