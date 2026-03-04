/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2026/02/13 09:51:43 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/13 09:51:43 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*parser(t_token *token, t_shell **shell)
{
	t_cmd *command_list;
	t_cmd *command;
	t_token *current_token;
	int code;
	int i;

	i = 0;
	code = 0;
	current_token = token;
	command_list = create_cmd();
	command = command_list;

	while (current_token)
	{
		if (current_token->type == TOKEN_WORD
			|| current_token->type == TOKEN_ENV)
			code = parse_words(&current_token, &i, &command, shell);
		else if (current_token->type == TOKEN_REDIR_IN
			|| current_token->type == TOKEN_REDIR_OUT
			|| current_token->type == TOKEN_APPEND
			|| current_token->type == TOKEN_HEREDOC)
			code = parse_redirs(&current_token, &command_list, &command, shell);
		else if (current_token->type == TOKEN_PIPE)
			code = parse_pipe(&current_token, &command, &command_list, shell);

		if (code != 0)
			return (NULL);
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
