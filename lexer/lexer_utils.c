/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 15:40:28 by aalbano           #+#    #+#             */
/*   Updated: 2026/02/25 19:43:34 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_space(const char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_env_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '_');
}

char	*get_type_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == TOKEN_APPEND)
		return ("APPEND");
	if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	if (type == TOKEN_ENV)
		return ("ENV");
	return ("UNKNOWN");
}

char	*get_quote_str(t_quote_type quote)
{
	if (quote == QUOTE_NONE)
		return ("NONE");
	if (quote == QUOTE_SINGLE)
		return ("SQUOTE");
	if (quote == QUOTE_DOUBLE)
		return ("DQUOTE");
	return ("UNKNOWN");
}

void	print_tokens(t_token *list)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = list;
	printf("--- Tokens ---\n");
	while (tmp)
	{
		printf("Token[%d]: %-10s | quote: %-6s | value: \"%s\"\n", i,
			get_type_str(tmp->type), get_quote_str(tmp->quote), tmp->value);
		tmp = tmp->next;
		i++;
	}
}

void	free_tokens(t_token *list)
{
	t_token *tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}