/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:51:53 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/03 10:26:30 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void update_pwd(t_shell **shell) {
	t_env *env = (*shell)->envs;
	char *new_dir = getcwd(NULL, 0);
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

void update_oldpwd(t_shell **shell, char *old_dir)
{
	t_env *env = (*shell)->envs;

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

int	builtin_cd(t_cmd *cmd, t_shell **shell)
{
	t_cmd_arg *current_arg;
	char *cur_dir;
	char *path;

	cur_dir = getcwd(NULL, 0);
	current_arg = cmd->args;
	if (cmd->argc > 2)
	{
		print_error("cd: too many arguments");
		return (1);
	}
	if (current_arg)
		current_arg = current_arg->next;
	if (!current_arg)
	{
		path = expand_env("HOME", (*shell)->envs);
		if (!path || path[0] == '\0')
		{
			free(path);
			print_error("cd: HOME not set");
			return (1);
		}
	}
	else
		path = ft_strdup(current_arg->value);
	if (chdir(path) < 0)
	{
		char *message = ft_strjoin("cd: ", path);
		message = ft_strjoin(message, ": ");
		message = ft_strjoin(message, strerror(errno));
		print_error(message);
		free(message);
		free(path);
		return (1);
	}
	update_pwd(shell);
	update_oldpwd(shell, cur_dir);
	free(path);
	return (0);
}