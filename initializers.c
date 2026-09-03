/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 10:44:44 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_shell(t_shell **shell, char **envp)
{
	if (!(*shell))
	{
		write(STDOUT_FILENO, "shell initialization: Fail", 26);
		exit(1);
	}
	(*shell)->envs = initialize_envs(envp);
	(*shell)->last_exit = 0;
	(*shell)->tokens = NULL;
	(*shell)->commands = NULL;
}

static void	parse_env_entry(char *entry, char **key, char **value)
{
	int	start;
	int	end;

	start = 0;
	end = 0;
	while (entry[start] != '=')
		start++;
	while (entry[start + end])
		end++;
	*key = ft_substr(entry, 0, start);
	*value = ft_substr(entry, start + 1, end);
}

t_env	*initialize_envs(char **envp)
{
	int		x;
	char	*key;
	char	*value;
	t_env	*head;

	head = NULL;
	x = 0;
	while (envp[x])
	{
		parse_env_entry(envp[x], &key, &value);
		push_env(&head, key, value);
		free(key);
		free(value);
		x++;
	}
	return (head);
}
