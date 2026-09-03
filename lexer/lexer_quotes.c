/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 15:39:59 by aalbano           #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_quoted(char *input, int *i, t_quote_type *quote_type)
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

char	*extract_env(char *input, int *i)
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

char	*extract_plain(char *input, int *i)
{
	int		start;
	char	*value;

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& !is_operator(input[*i]) && !is_quote(input[*i])
		&& input[*i] != '$')
		(*i)++;
	if (*i > start)
		value = ft_substr(input, start, *i - start);
	else
		value = NULL;
	return (value);
}

int	is_word_char(char c)
{
	if (!c || c == ' ' || c == '\t')
		return (0);
	if (is_operator(c))
		return (0);
	return (1);
}

char	*join_and_free(char *result, char *part)
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
