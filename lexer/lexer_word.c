/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token_type	get_word_type(char *value, int has_env)
{
	if (has_env && value && value[0] == '$' && ft_strlen(value) > 1)
		return (TOKEN_ENV);
	return (TOKEN_WORD);
}

static char	*word_part(char *input, int *i, t_quote_type *qt, int *has_env)
{
	char	*part;

	if (input[*i] == '\'' || input[*i] == '"')
		part = extract_quoted(input, i, qt);
	else if (input[*i] == '$')
	{
		part = extract_env(input, i);
		if (part && part[0] == '$' && ft_strlen(part) > 1)
			*has_env = 1;
	}
	else
		part = extract_plain(input, i);
	return (part);
}

void	tokenize_word(char *input, int *i, t_token **list)
{
	char			*result;
	t_quote_type	qtype;
	int				has_env;

	result = NULL;
	qtype = QUOTE_NONE;
	has_env = 0;
	while (is_word_char(input[*i]))
	{
		result = join_and_free(result,
				word_part(input, i, &qtype, &has_env));
	}
	if (result)
		add_token(list, new_token(get_word_type(result, has_env),
				qtype, result));
	free(result);
}
