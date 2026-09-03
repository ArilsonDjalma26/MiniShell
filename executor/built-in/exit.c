/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:54:42 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/12 20:19:55 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_space(int c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

static int	is_valid(char *arg)
{
	int	i;
	int	has_digit;

	i = 0;
	has_digit = 0;
	if (!arg || !arg[0])
		return (0);
	while (is_space(arg[i]))
		i++;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (ft_isdigit(arg[i]))
	{
		has_digit = 1;
		i++;
	}
	if (!has_digit)
		return (0);
	while (is_space(arg[i]))
		i++;
	if (arg[i])
		return (0);
	return (1);
}

static void	exit_numeric_error(t_cmd *cmd, t_shell **shell)
{
	char	*temp;
	char	*error_msg;

	temp = ft_strjoin("exit: ", cmd->args->next->value);
	error_msg = ft_strjoin(temp, ": numeric argument required");
	free(temp);
	print_error(error_msg);
	free(error_msg);
	close_shell(shell, 2);
}

static int	exit_too_many(t_shell **shell)
{
	char	*error_msg;

	error_msg = ft_strjoin("exit: ", "too many arguments");
	print_error(error_msg);
	free(error_msg);
	(*shell)->last_exit = 1;
	return (1);
}

int	builtin_exit(t_cmd *cmd, t_shell **shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	if (!cmd->args->next)
		close_shell(shell, (*shell)->last_exit);
	if (!is_valid(cmd->args->next->value))
		exit_numeric_error(cmd, shell);
	else if (cmd->argc > 2)
		return (exit_too_many(shell));
	else
		close_shell(shell, ft_atoi(cmd->args->next->value, shell));
	return (1);
}
