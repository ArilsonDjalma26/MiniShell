/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freezers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:51:37 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/13 09:51:38 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_arg_list(t_cmd_arg *arg_list)
{
	t_cmd_arg	*current_arg;
	t_cmd_arg	*temp;

	current_arg = arg_list;
	while (current_arg)
	{
		temp = current_arg->next;
		free(current_arg->value);
		free(current_arg);
		current_arg = temp;
	}
}

void	free_redir_list(t_redir *redir_list)
{
	t_redir	*current_redir;
	t_redir	*temp;

	current_redir = redir_list;
	while (current_redir)
	{
		temp = current_redir->next;
		free(current_redir->file);
		free(current_redir);
		current_redir = temp;
	}
}

void	free_cmd_list(t_cmd *command_list)
{
	t_cmd	*current_cmd;
	t_cmd	*temp;

	current_cmd = command_list;
	while (current_cmd)
	{
		temp = current_cmd->next;
		free_arg_list(current_cmd->args);
		free_redir_list(current_cmd->redirs);
		free(current_cmd);
		current_cmd = temp;
	}
}

void	free_env_list(t_env *env_list)
{
	t_env	*current_env;
	t_env	*temp;

	current_env = env_list;
	while (current_env)
	{
		temp = current_env->next;
		free(current_env->key);
		free(current_env->value);
		free(current_env);
		current_env = temp;
	}
}
