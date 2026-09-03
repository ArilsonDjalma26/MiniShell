/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:41:15 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/12 20:04:26 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exit_numeric_error(const char *s, t_shell **shell)
{
	char	*temp;
	char	*error_msg;

	temp = ft_strjoin("exit: ", s);
	error_msg = ft_strjoin(temp, ": numeric argument required");
	free(temp);
	print_error(error_msg);
	free(error_msg);
	close_shell(shell, 2);
}

int	is_overflow(int *signal, int *i, long long *res, char *s)
{
	int	digit;

	digit = s[*i] - '0';
	if (*signal == 1)
	{
		if (*res > LLONG_MAX / 10)
			return (1);
		if (*res == LLONG_MAX / 10 && digit > 7)
			return (1);
	}
	else
	{
		if (*res > (LLONG_MAX / 10))
			return (1);
		if (*res == (LLONG_MAX / 10) && digit > 8)
			return (1);
	}
	return (0);
}

static int	ft_skip_sign(const char *nptr, int *i, int *sign)
{
	while (nptr[*i] == 32 || (nptr[*i] >= 9 && nptr[*i] <= 13))
		(*i)++;
	if (nptr[*i] == '-')
	{
		*sign = -1;
		(*i)++;
	}
	else if (nptr[*i] == '+')
		(*i)++;
	return (0);
}

int	ft_atoi(const char *nptr, t_shell **shell)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	if (!nptr)
		return (0);
	ft_skip_sign(nptr, &i, &sign);
	while (ft_isdigit(nptr[i]))
	{
		if (is_overflow(&sign, &i, &result, (char *)nptr))
		{
			exit_numeric_error(nptr, shell);
		}
		result = (result * 10) + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}
