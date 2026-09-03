/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	toggle_quote_state(int *state, t_token *token, int i)
{
	if (token->value[i] == '\'' && *state == 0)
		*state = 1;
	else if (token->value[i] == '\'' && *state == 1)
		*state = 0;
	else if (token->value[i] == '"' && *state == 0)
		*state = 2;
	else if (token->value[i] == '"' && *state == 2)
		*state = 0;
}

void	no_expand(t_token *token, int *i, char **final_string)
{
	*final_string = append_chr(*final_string, token->value[*i]);
	(*i)++;
}
