/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 18:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
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

static char	*try_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!full)
		return (NULL);
	if (access(full, F_OK | X_OK) == 0)
		return (full);
	free(full);
	return (NULL);
}

static char	*search_in_paths(char *cmd, char **dirs)
{
	int		i;
	char	*result;

	i = 0;
	while (dirs[i])
	{
		result = try_path(dirs[i], cmd);
		if (result)
			return (result);
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
