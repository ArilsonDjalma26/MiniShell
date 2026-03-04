/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 18:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/27 15:06:25 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_path_value(t_env *envs)
{
	t_env	*cur;

	cur = envs;
	while (cur)
	{
		if (ft_strcmp(cur->key, "PATH") == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

static char	*search_in_paths(char *cmd, char **dirs)
{
	int		i;
	char	*tmp;
	char	*full;

	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		if (!tmp)
		{
			i++;
			continue ;
		}
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full)
		{
			i++;
			continue ;
		}
		if (access(full, F_OK | X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, t_env *envs)
{
	char	*path_val;
	char	**dirs;
	char	*result;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_val = get_path_value(envs);
	if (!path_val)
		return (NULL);
	dirs = ft_split(path_val, ':');
	if (!dirs)
		return (NULL);
	result = search_in_paths(cmd, dirs);
	ft_free_split(dirs);
	return (result);
}
