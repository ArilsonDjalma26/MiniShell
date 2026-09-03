/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	redir_syntax_err(t_cmd **list, t_shell **shell, char *msg)
{
	free_cmd_list(*list);
	*list = NULL;
	print_error(msg);
	(*shell)->last_exit = 2;
	return (2);
}

static int	check_redir_next(t_token **cur, t_cmd **list, t_shell **shell)
{
	if (!(*cur)->next)
		return (redir_syntax_err(list, shell,
				"syntax error near unexpected token `newline'"));
	if ((*cur)->next->type != TOKEN_WORD && (*cur)->next->type != TOKEN_ENV)
		return (redir_syntax_err(list, shell,
				"syntax error near unexpected token `'"));
	return (0);
}

static int	handle_heredoc(t_token **cur, t_cmd **list,
		t_cmd **cmd, t_shell **shell)
{
	char	*delim;
	char	*content;
	char	*expanded;

	delim = remove_quotes((*cur)->next->value);
	content = read_heredoc(delim);
	free(delim);
	if (!content)
	{
		free_cmd_list(*list);
		*list = NULL;
		(*shell)->last_exit = 130;
		return (2);
	}
	if ((*cur)->next->quote == QUOTE_NONE)
	{
		expanded = expand_heredoc_str(content, *shell);
		free(content);
		content = expanded;
	}
	push_redir(&(*cmd)->redirs, TOKEN_HEREDOC, content);
	free(content);
	return (0);
}

static int	handle_file_redir(t_token **cur, t_cmd **list,
		t_cmd **cmd, t_shell **shell)
{
	char	*expanded;

	expanded = expand_word((*cur)->next, shell);
	if (!expanded)
	{
		free_cmd_list(*list);
		*list = NULL;
		return (2);
	}
	push_redir(&(*cmd)->redirs, (*cur)->type, expanded);
	free(expanded);
	return (0);
}

int	parse_redirs(t_token **cur, t_cmd **list, t_cmd **cmd, t_shell **shell)
{
	int	code;

	code = check_redir_next(cur, list, shell);
	if (code != 0)
		return (code);
	if ((*cur)->type == TOKEN_HEREDOC)
		code = handle_heredoc(cur, list, cmd, shell);
	else
		code = handle_file_redir(cur, list, cmd, shell);
	if (code != 0)
		return (code);
	*cur = (*cur)->next->next;
	return (0);
}
