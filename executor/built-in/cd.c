/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:51:53 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	update_pwd(t_shell **shell)
{
	t_env	*env;
	char	*new_dir;

	env = (*shell)->envs;
	new_dir = getcwd(NULL, 0);
	while (env)
	{
		if (ft_strcmp(env->key, "PWD") == 0)
		{
			free(env->value);
			env->value = new_dir;
			return ;
		}
		env = env->next;
	}
	free(new_dir);
}

void	update_oldpwd(t_shell **shell, char *old_dir)
{
	t_env	*env;

	env = (*shell)->envs;
	while (env)
	{
		if (ft_strcmp(env->key, "OLDPWD") == 0)
		{
			free(env->value);
			env->value = old_dir;
			return ;
		}
		env = env->next;
	}
	free(old_dir);
}

static int	cd_chdir_error(char *path, char *cur_dir)
{
	char	*message;
	char	*tmp;

	message = ft_strjoin("cd: ", path);
	tmp = ft_strjoin(message, ": ");
	free(message);
	message = ft_strjoin(tmp, strerror(errno));
	free(tmp);
	print_error(message);
	free(message);
	free(path);
	free(cur_dir);
	return (1);
}

static char	*cd_get_path(t_cmd *cmd, t_shell **shell, char *cur_dir)
{
	t_cmd_arg	*arg;
	char		*path;

	arg = cmd->args;
	if (arg)
		arg = arg->next;
	if (!arg)
	{
		path = expand_env("HOME", (*shell)->envs);
		if (!path || path[0] == '\0')
		{
			free(path);
			free(cur_dir);
			print_error("cd: HOME not set");
			return (NULL);
		}
		return (path);
	}
	return (ft_strdup(arg->value));
}

int	builtin_cd(t_cmd *cmd, t_shell **shell)
{
	char	*cur_dir;
	char	*path;

	cur_dir = getcwd(NULL, 0);
	if (cmd->argc > 2)
	{
		free(cur_dir);
		print_error("cd: too many arguments");
		return (1);
	}
	path = cd_get_path(cmd, shell, cur_dir);
	if (!path)
		return (1);
	if (chdir(path) < 0)
		return (cd_chdir_error(path, cur_dir));
	update_pwd(shell);
	update_oldpwd(shell, cur_dir);
	free(path);
	return (0);
}
