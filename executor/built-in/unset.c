/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:54:14 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	unset_free_node(t_env *node)
{
	free(node->key);
	free(node->value);
	free(node);
}

static void	unset_one(t_shell **shell, char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = (*shell)->envs;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(key, cur->key) == 0)
		{
			if (!prev)
				(*shell)->envs = cur->next;
			else
				prev->next = cur->next;
			unset_free_node(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	builtin_unset(t_cmd *cmd, t_shell **shell)
{
	t_cmd_arg	*arg;

	if (!(*shell)->envs || !cmd->args->next)
		return (0);
	arg = cmd->args->next;
	while (arg)
	{
		unset_one(shell, arg->value);
		arg = arg->next;
	}
	return (0);
}
