/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <elfranco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 12:44:53 by elfranco          #+#    #+#             */
/*   Updated: 2026/01/13 14:53:23 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

//=========================ENUMS====================//

typedef enum e_lexer_state
{
    LEXER_DEFAULT,
    LEXER_IN_SINGLE_QUOTE,
    LEXER_IN_DOUBLE_QUOTE
} t_lexer_state;

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC
}   t_token_type;

//=========================structs====================//

typedef struct s_token
{
	t_token_type type;
    char    *value;
	struct s_token *next;
}   t_token;
//=========================functions====================//
void    skipe_space(const char *input, int *i);
int is_operator(char c);

//=========================utils====================//

char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *src);
size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
