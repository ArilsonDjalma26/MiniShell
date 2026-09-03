/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 12:50:52 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*lexer(char *input)
{
	t_token	*list;
	int		i;

	i = 0;
	list = NULL;
	while (input[i])
	{
		skip_space(input, &i);
		if (!input[i])
			break ;
		if (is_operator(input[i]))
			tokenize_operator(input, &i, &list);
		else
			tokenize_word(input, &i, &list);
	}
	return (list);
}
