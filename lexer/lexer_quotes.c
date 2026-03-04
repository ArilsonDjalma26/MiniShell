/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 15:39:59 by aalbano           #+#    #+#             */
/*   Updated: 2026/02/25 19:43:25 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*extract_quoted(char *input, int *i, t_quote_type *quote_type)
{
	char	quote;
	int		start;
	char	*value;

	start = *i;
	quote = input[*i];
	if (quote == '"')
		*quote_type = QUOTE_DOUBLE;
	else if (quote == '\'')
		*quote_type = QUOTE_SINGLE;
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	value = ft_substr(input, start, *i - start);
	return (value);
}

static char	*extract_env(char *input, int *i)
{
	int		start;
	char	*value;

	start = *i;
	(*i)++;
	if (!input[*i] || (!is_env_char(input[*i]) && input[*i] != '?'))
		return (ft_strdup("$"));
	if (input[*i] == '?')
		(*i)++;
	else
	{
		while (input[*i] && is_env_char(input[*i]))
			(*i)++;
	}
	value = ft_substr(input, start, *i - start);
	return (value);
}
static char	*extract_plain(char *input, int *i)
{
	int		start;
	char	*value;

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& !is_operator(input[*i]) && !is_quote(input[*i]) && input[*i] != '$')
		(*i)++;
	if (*i > start)
		value = ft_substr(input, start, *i - start);
	else
		value = NULL;
	return (value);
}
static int	is_word_char(char c)
{
	if (!c || c == ' ' || c == '\t')
		return (0);
	if (is_operator(c))
		return (0);
	return (1);
}
static char	*join_and_free(char *result, char *part)
{
	char	*new_result;

	if (!part)
		return (result);
	if (!result)
		return (part);
	new_result = ft_strjoin(result, part);
	free(result);
	free(part);
	return (new_result);
}
static t_token_type	get_word_type(char *value, int has_env)
{
	if (has_env && value && value[0] == '$' && ft_strlen(value) > 1)
		return (TOKEN_ENV);
	return (TOKEN_WORD);
}
void	tokenize_word(char *input, int *i, t_token **list)
{
	char			*result;
	char			*part;
	t_quote_type	qtype;
	int				has_env;

	result = 0;
	qtype = QUOTE_NONE;
	has_env = 0;
	while (is_word_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			part = extract_quoted(input, i, &qtype);
		}
		else if (input[*i] == '$')
		{
			part = extract_env(input, i);
			if (part && part[0] == '$' && ft_strlen(part) > 1)
				has_env = 1;
		}
		else
			part = extract_plain(input, i);
		result = join_and_free(result, part);
	}
	if (result)
		add_token(list, new_token(get_word_type(result, has_env), qtype,
				result));
}
