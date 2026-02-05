/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbano <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:07:03 by aalbano           #+#    #+#             */
/*   Updated: 2026/02/05 16:07:06 by aalbano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef LEXER_H
#define LEXER_H

#include "../minishell.h"

//lexer_tokens.c

//lexer_quotes.c

//lexer_utils.c

t_token *new_token(t_token_type type, char *value);
t_token add_token(t_token **list, t_token *new);

#endif