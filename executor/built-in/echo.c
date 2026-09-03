/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:24:06 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_newliner(t_cmd_arg *arg)
{
	size_t	i;

	if (!arg || arg->value[0] != '-')
		return (0);
	i = 1;
	while (arg->value[i] && arg->value[i] == 'n')
		i++;
	if (i != ft_strlen(arg->value))
		return (0);
	return (1);
}

int	builtin_echo(t_cmd *cmd)
{
	t_cmd_arg	*arg;
	int			newline;

	arg = cmd->args->next;
	newline = 1;
	while (arg && is_newliner(arg))
	{
		newline = 0;
		arg = arg->next;
	}
	while (arg)
	{
		write(STDOUT_FILENO, arg->value, ft_strlen(arg->value));
		if (arg->next)
			write(STDOUT_FILENO, " ", 1);
		arg = arg->next;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
