/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:12:14 by elfranco          #+#    #+#             */
/*   Updated: 2025/06/24 13:13:44 by elfranco         ###   ########.fr       */
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

char	*ft_itoa(int n)
{
	long		number;
	int			numbers_len;
	char		*result;

	number = (long)n;
	numbers_len = ft_count_numbers(n);
	result = (char *)malloc((numbers_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[numbers_len] = '\0';
	if (n == 0)
		result[0] = '0';
	if (n < 0)
	{
		result[0] = '-';
		number = -number;
	}
	numbers_len--;
	while (number > 0)
	{
		result[numbers_len] = (number % 10) + '0';
		number = number / 10;
		numbers_len--;
	}
	return (result);
}
