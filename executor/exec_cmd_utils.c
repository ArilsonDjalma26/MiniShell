/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_partial_arr(char **arr, int i)
{
	while (i > 0)
		free(arr[--i]);
	free(arr);
}

static int	env_count(t_env *envs)
{
	int	count;

	count = 0;
	while (envs)
	{
		count++;
		envs = envs->next;
	}
	return (count);
}

char	**env_to_array(t_env *envs)
{
	t_env	*cur;
	char	**arr;
	char	*tmp;
	int		i;

	arr = (char **)malloc(sizeof(char *) * (env_count(envs) + 1));
	if (!arr)
		return (NULL);
	cur = envs;
	i = 0;
	while (cur)
	{
		tmp = ft_strjoin(cur->key, "=");
		arr[i] = ft_strjoin(tmp, cur->value);
		free(tmp);
		if (!arr[i])
			return (free_partial_arr(arr, i), NULL);
		cur = cur->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

void	close_shell(t_shell **shell, int exit_code)
{
	free_cmd_list((*shell)->commands);
	free_env_list((*shell)->envs);
	free_tokens((*shell)->tokens);
	free(*shell);
	exit(exit_code);
}
