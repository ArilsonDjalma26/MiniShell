/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:06:46 by aalbano           #+#    #+#             */
/*   Updated: 2026/03/12 16:20:15 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(t_token_type type, t_quote_type quote, char *value)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->quote = quote;
	tok->value = ft_strdup(value);
	tok->next = NULL;
	return (tok);
}

void	add_token(t_token **list, t_token *new_token)
{
	t_token	*tmp;

	if (!new_token)
		return ;
	if (!*list)
	{
		*list = new_token;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
}

static void	tokenize_redir(char *input, int *i, t_token **list)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(list, new_token(TOKEN_APPEND, QUOTE_NONE, ">>"));
		(*i) += 2;
	}
	else if (input[*i] == '>')
	{
		add_token(list, new_token(TOKEN_REDIR_OUT, QUOTE_NONE, ">"));
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(list, new_token(TOKEN_HEREDOC, QUOTE_NONE, "<<"));
		(*i) += 2;
	}
	else if (input[*i] == '<')
	{
		add_token(list, new_token(TOKEN_REDIR_IN, QUOTE_NONE, "<"));
		(*i)++;
	}
}

void	tokenize_operator(char *input, int *i, t_token **list)
{
	if (input[*i] == '|')
	{
		add_token(list, new_token(TOKEN_PIPE, QUOTE_NONE, "|"));
		(*i)++;
	}
	else
		tokenize_redir(input, i, list);
}
