/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:12:14 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_numbers(int n)
{
	long	number;
	int		i;

	number = (long)n;
	i = 0;
	if (n < 0)
		number = -number;
	if (n == 0)
		return (1);
	while (number > 0)
	{
		number = number / 10;
		i++;
	}
	if (n < 0)
		i++;
	return (i);
}

static void	ft_fill_result(char *result, long number, int len)
{
	len--;
	while (number > 0)
	{
		result[len] = (number % 10) + '0';
		number = number / 10;
		len--;
	}
}

char	*ft_itoa(int n)
{
	long	number;
	int		len;
	char	*result;

	number = (long)n;
	len = ft_count_numbers(n);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[len] = '\0';
	if (n == 0)
		result[0] = '0';
	if (n < 0)
	{
		result[0] = '-';
		number = -number;
	}
	ft_fill_result(result, number, len);
	return (result);
}
