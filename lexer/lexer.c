/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <elfranco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 12:50:52 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/11 12:00:00 by aalbano          ###   ########.fr       */
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