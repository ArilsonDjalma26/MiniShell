/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 11:52:45 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/03 16:33:56 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*join_heredoc_line(char *content, char *line)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(content, line);
	free(content);
	result = ft_strjoin(tmp, "\n");
	free(tmp);
	return (result);
}

char	*read_heredoc(char *delimiter)
{
	char	*line;
	char	*content;

	content = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDERR_FILENO,
				"minishell: warning: here-document delimited by end-of-file\n",
				58);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		content = join_heredoc_line(content, line);
		free(line);
	}
	return (content);
}

int	parse_words(t_token **current_token, int *i, t_cmd **command,
		t_shell **shell)
{
	char	**word_splited;

	word_splited = NULL;
	if ((*current_token)->quote == QUOTE_NONE)
	{
		*i = 0;
		if (!expand_word(*current_token, shell))
			return (2);
		word_splited = ft_split(remove_quotes(expand_word(*current_token,
						shell)), ' ');
		while (word_splited[*i])
		{
			push_arg(&(*command)->args, word_splited[*i]);
			(*command)->argc++;
			(*i)++;
		}
		ft_free_split(word_splited);
	}
	else
	{
		if (!expand_word(*current_token, shell))
			return (2);
		push_arg(&(*command)->args, remove_quotes(expand_word(*current_token,
					shell)));
		(*command)->argc++;
	}
	*current_token = (*current_token)->next;
	return (0);
}

int	parse_redirs(t_token **current_token, t_cmd **command_list,
		t_cmd **command, t_shell **shell)
{
	if (!(*current_token)->next)
	{
		free_cmd_list(*command_list);
		print_error("syntax error near unexpected token `newline'");
		(*shell)->last_exit = 2;
		return (2);
	}
	else if ((*current_token)->next->type != TOKEN_WORD
		&& (*current_token)->next->type != TOKEN_ENV)
	{
		free_cmd_list(*command_list);
		print_error("syntax error near unexpected token `'");
		(*shell)->last_exit = 2;
		return (2);
	}
	if ((*current_token)->type == TOKEN_HEREDOC)
	{
		{
			char	*delim;
			char	*content;
			char	*expanded;
			int		has_quotes;

			has_quotes = ((*current_token)->next->quote != QUOTE_NONE);
			delim = remove_quotes((*current_token)->next->value);
			content = read_heredoc(delim);
			free(delim);
			if (!has_quotes)
			{
				expanded = expand_heredoc_str(content, *shell);
				free(content);
				content = expanded;
			}
			push_redir(&(*command)->redirs, TOKEN_HEREDOC, content);
			free(content);
		}
	}
	else
	{
		if (!expand_word((*current_token)->next, shell))
		{
			free_cmd_list(*command_list);
			return (2);
		}
		push_redir(&(*command)->redirs, (*current_token)->type,
			remove_quotes(expand_word((*current_token)->next, shell)));
	}
	(*current_token) = (*current_token)->next->next;
	return (0);
}

int	parse_pipe(t_token **current_token, t_cmd **command,
		t_cmd **command_list, t_shell **shell)
{
	if (((*command)->argc == 0 && !(*command)->redirs)
		|| !(*current_token)->next)
	{
		free_cmd_list(*command_list);
		print_error("syntax error near unexpected token `|'");
		(*shell)->last_exit = 2;
		return (2);
	}
	(*command)->next = create_cmd();
	*command = (*command)->next;
	*current_token = (*current_token)->next;
	return (0);
}
