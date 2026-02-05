/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbano <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:06:46 by aalbano           #+#    #+#             */
/*   Updated: 2026/02/05 16:06:51 by aalbano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *new_token(t_token_type type, char *value)
{
    t_token *tok;

    tok = malloc(sizeof(t_token));
    if(!tok)
        return ;
    tok->type = type;
    tok->value = value;
    tok->next = NULL;

    return (tok);
}
void add_token(t_token **list, t_token *new)
{
    t_token *tmp;

    if(!list)
    {
        list = new;
    }
    tmp = new;
    while(tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}
void    tokenize_operator(char *input, int *i, t_token **list)
{
    
}
void    tokenize_word()
{

}
