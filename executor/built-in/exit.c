/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:54:42 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/03 14:45:37 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int is_valid(char *arg) {
	int i = 0;

	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_cmd *cmd, t_shell **shell)
{
	t_cmd_arg *current_arg;

	write(STDOUT_FILENO, "exit\n", 5);
	if (!cmd->args->next)
	    exit((*shell)->last_exit);
	current_arg = cmd->args->next;
	
	if (!is_valid(current_arg->value))
	{
	    char *error_msg = ft_strjoin("exit: ", current_arg->value);
		error_msg = ft_strjoin(error_msg, ": numeric argument required");
		print_error(error_msg);
		free(error_msg);
		exit(2);
	}
	else if (cmd->argc > 2)
	{
		char *error_msg = ft_strjoin("exit: ", "too many arguments");
		print_error(error_msg);
		free(error_msg);
		(*shell)->last_exit = 1;
		return (1);
	}
	else
	{
    	exit(atoi(current_arg->value));
	}
	return (1);
}