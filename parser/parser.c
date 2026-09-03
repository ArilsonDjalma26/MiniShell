/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:51:43 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/12 17:45:51 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_redir(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static int	parse_token(t_token **cur, t_cmd **cmd, t_cmd **list, t_shell **sh)
{
	int	i;
	int	code;

	i = 0;
	code = 0;
	if ((*cur)->type == TOKEN_WORD || (*cur)->type == TOKEN_ENV)
		code = parse_words(cur, &i, cmd, sh);
	else if (is_redir((*cur)->type))
		code = parse_redirs(cur, list, cmd, sh);
	else if ((*cur)->type == TOKEN_PIPE)
		code = parse_pipe(cur, cmd, list, sh);
	return (code);
}

t_cmd	*parser(t_token *token, t_shell **shell)
{
	t_cmd	*command_list;
	t_cmd	*command;
	t_token	*curr_token;

	curr_token = token;
	command_list = create_cmd();
	command = command_list;
	while (curr_token)
	{
		if (parse_token(&curr_token, &command, &command_list, shell) != 0)
		{
			free_cmd_list(command_list);
			command_list = NULL;
			return (NULL);
		}
	}
	return (command_list);
}
