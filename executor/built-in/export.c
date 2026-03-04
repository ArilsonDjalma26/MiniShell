/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:54:31 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/03 12:02:12 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int is_valid_identifier(char *arg) {
	int i = 0;
	
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

t_env	*sort_list(t_env *envs)
{
	t_env	*current_env;
	int		swapped;
	char	*tmp_key;
	char	*tmp_value;

	if (!envs)
		return (NULL);

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current_env = envs;
		while (current_env->next)
		{
			if (ft_strcmp(current_env->key, current_env->next->key) > 0)
			{
				tmp_key = current_env->key;
				tmp_value = current_env->value;

				current_env->key = current_env->next->key;
				current_env->value = current_env->next->value;

				current_env->next->key = tmp_key;
				current_env->next->value = tmp_value;

				swapped = 1;
			}
			current_env = current_env->next;
		}
	}
	return (envs);
}

void update_env(t_shell **shell, char *key, char *value)
{
	t_env *env = (*shell)->envs;

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

int env_exists(t_shell **shell, char *key)
{
	t_env *env = (*shell)->envs;

	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

int	builtin_export(t_cmd *cmd, t_shell **shell)
{
	int start;
	int end;
	t_cmd_arg *current_arg;
	t_env *current_env;
	
	start = 0;
	end = 0;
	current_arg = cmd->args->next;
	if (!current_arg)
	{
		current_env = sort_list((*shell)->envs);
		while (current_env)
		{
			if (current_env->value != NULL)
				printf("declare -x %s=\"%s\"\n", current_env->key, current_env->value);
			else
				printf("declare -x %s\n", current_env->key);
			current_env = current_env->next;
		}
		return (0);
	}

	while (current_arg)
	{
		start = 0;
		end = 0;
		if (!is_valid_identifier(current_arg->value))
		{
			char *msg = ft_strjoin("export: ", current_arg->value);
			msg = ft_strjoin(msg, ": not a valid identifier");
			print_error(msg);
			return (1);
		}
		
		while (current_arg->value[start] && current_arg->value[start] != '=')
			start++;
		while (current_arg->value[start + end])
			end++;
			
		if (env_exists(shell, ft_substr(current_arg->value, 0, start)))
		{
			if (ft_strchr(current_arg->value, '='))
				update_env(shell, ft_substr(current_arg->value, 0, start), ft_substr(current_arg->value, start + 1, end));
		}
		else
		{
			if (ft_strchr(current_arg->value, '='))
				push_env(&(*shell)->envs, ft_substr(current_arg->value, 0, start), ft_substr(current_arg->value, start + 1, end));
			else
				push_env(&(*shell)->envs, ft_substr(current_arg->value, 0, start), NULL);
		}
		current_arg = current_arg->next;
	}
	return (0);
}