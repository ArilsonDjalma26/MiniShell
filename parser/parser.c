/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:51:43 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/13 09:51:43 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>


t_cmd	*parser(t_token *token)
{
	t_cmd *command_list;
	t_cmd *command;
	t_token *current_token;

	current_token = token;
	command_list = (t_cmd *)malloc(sizeof(t_cmd));
	if (!command_list)
		return (NULL);
	command = command_list;
	command->args = NULL;
	command->redirs = NULL;
	command->argc = 0;
	command->next = NULL;

	while (current_token)
	{
		if (current_token->type == TOKEN_WORD
			|| current_token->type == TOKEN_ENV)
		{
			push_arg(&command->args, current_token->value);
			command->argc++;
			current_token = current_token->next;
		}
		else if (current_token->type == TOKEN_REDIR_IN
			|| current_token->type == TOKEN_REDIR_OUT
			|| current_token->type == TOKEN_APPEND
			|| current_token->type == TOKEN_HEREDOC)
		{
			if (!current_token->next)
			{
				free_cmd_list(command_list);
				print_error("syntax error near unexpected token `newline'");
				return (NULL);
			}
			else if (current_token->next->type != TOKEN_WORD
				&& current_token->next->type != TOKEN_ENV)
			{
				free_cmd_list(command_list);
				print_error("syntax error near unexpected token `'");
				return (NULL);
			}
			push_redir(&command->redirs, current_token->type,
				current_token->next->value);
			current_token = current_token->next->next;
		}
		else if (current_token->type == TOKEN_PIPE)
		{
			if ((command->argc == 0 && !command->redirs)
				|| !current_token->next)
			{
				free_cmd_list(command_list);
				print_error("syntax error near unexpected token `|'");
				return (NULL);
			}
			command->next = create_cmd();
			command = command->next;
			current_token = current_token->next;
		}
	}
	return (command_list);
}

static const char	*redir_str(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return ("<");
	if (type == TOKEN_REDIR_OUT)
		return (">");
	if (type == TOKEN_APPEND)
		return (">>");
	if (type == TOKEN_HEREDOC)
		return ("<<");
	return ("?");
}

void	print_cmds(t_cmd *cmd_list)
{
	int i;
	t_cmd_arg *arg;
	t_redir *redir;

	i = 0;
	while (cmd_list)
	{
		printf("─── Cmd[%d] (%d args) ───\n", i, cmd_list->argc);
		arg = cmd_list->args;
		while (arg)
		{
			printf("  arg: \"%s\"\n", arg->value);
			arg = arg->next;
		}
		redir = cmd_list->redirs;
		while (redir)
		{
			printf("  redir: %s \"%s\"\n", redir_str(redir->type),
				redir->file);
			redir = redir->next;
		}
		cmd_list = cmd_list->next;
		i++;
	}
}