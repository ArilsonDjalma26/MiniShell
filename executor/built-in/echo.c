/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:24:06 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/27 15:25:32 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_newliner(t_cmd_arg *current_arg)
{
	size_t	i;
	int		isNewliner;

	i = 0;
	isNewliner = 0;
	if (current_arg && current_arg->value[0] == '-')
	{
		i = 1;
		while (current_arg->value[i] && current_arg->value[i] == 'n')
			i++;
		if (i != ft_strlen(current_arg->value))
			isNewliner = 0;
		else
			isNewliner = 1;
	}
	return (isNewliner);
}

int builtin_echo(t_cmd *cmd)
{
    t_cmd_arg *arg;
    int newline;

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
