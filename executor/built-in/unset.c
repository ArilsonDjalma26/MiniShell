/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:54:14 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/02 16:40:35 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_unset(t_cmd *cmd, t_shell **shell)
{
	t_env		*current_env;
	t_env		*prev_env;
	t_cmd_arg	*current_arg;
	t_env		*temp_env;
	
	if (!(*shell)->envs)
		return (0);
	if (!cmd->args->next)
		return (0);

	current_arg = cmd->args->next;
	while (current_arg)
	{
		current_env = (*shell)->envs;
		prev_env = NULL;

		while (current_env)
		{
			if (ft_strcmp(current_arg->value, current_env->key) == 0)
			{
				temp_env = current_env;

				if (prev_env == NULL)
					(*shell)->envs = current_env->next;
				else
					prev_env->next = current_env->next;

				current_env = current_env->next;

				free(temp_env->key);
				free(temp_env->value);
				free(temp_env);
			}
			else
			{
				prev_env = current_env;
				current_env = current_env->next;
			}
		}
		current_arg = current_arg->next;
	}
	return (0);
}