/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:54:31 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_identifier(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (0);
	if (ft_strlen(arg) == 0)
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!(ft_isalpha(arg[i]) || arg[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	swap_env(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = a->key;
	tmp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_value;
}

t_env	*sort_list(t_env *envs)
{
	t_env	*cur;
	int		swapped;

	if (!envs)
		return (NULL);
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		cur = envs;
		while (cur->next)
		{
			if (ft_strcmp(cur->key, cur->next->key) > 0)
			{
				swap_env(cur, cur->next);
				swapped = 1;
			}
			cur = cur->next;
		}
	}
	return (envs);
}

void	update_env(t_shell **shell, char *key, char *value)
{
	t_env	*env;

	env = (*shell)->envs;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = value;
			return ;
		}
		env = env->next;
	}
	free(value);
}

int	env_exists(t_shell **shell, char *key)
{
	t_env	*env;

	env = (*shell)->envs;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}
