/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 11:25:45 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/03 11:50:45 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_env(t_shell *shell)
{
	t_env *cur;

	if (!shell->envs)
	{
		write(STDERR_FILENO, "env: No environment variables\n", 29);
		return (-1);
	}
	cur = shell->envs;
	while (cur)
	{
		if (cur->value || (cur->value && cur->value[0] == '\0'))
		{
			write(STDOUT_FILENO, cur->key, ft_strlen(cur->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, cur->value, ft_strlen(cur->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		cur = cur->next;
	}
	return (0);
}