/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 12:44:53 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/13 09:51:18 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

//=========================ENUMS====================//

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV
}						t_token_type;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}						t_quote_type;

//=========================STRUCTS====================//

typedef struct s_token
{
	t_token_type		type;
	t_quote_type		quote;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_cmd_arg
{
	char				*value;
	struct s_cmd_arg	*next;
}						t_cmd_arg;

typedef struct s_redir
{
	t_token_type		type;
	char				*file;
	struct s_redir		*next;
}						t_redir;

typedef struct s_cmd
{
	t_cmd_arg			*args;
	int					argc;
	t_redir				*redirs;
	struct s_cmd		*next;
}						t_cmd;

//=========================LEXER====================//

t_token					*lexer(char *input);
void					skip_space(const char *input, int *i);
int						is_operator(char c);
int						is_quote(char c);
int						is_env_char(char c);
t_token					*new_token(t_token_type type, t_quote_type quote,
							char *value);
void					add_token(t_token **list, t_token *new_tok);
void					tokenize_operator(char *input, int *i, t_token **list);
void					tokenize_word(char *input, int *i, t_token **list);
void					print_tokens(t_token *list);
void					free_tokens(t_token *list);
char					*get_type_str(t_token_type type);
char					*get_quote_str(t_quote_type quote);

//=========================PARSER====================//

void					push_arg(t_cmd_arg **head, char *new_value);
void					print_error(char *message);
void					push_redir(t_redir **head, t_token_type type,
							char *file);
t_cmd					*create_cmd(void);
t_cmd					*parser(t_token *token);
void					print_cmds(t_cmd *cmd_list);
void					free_arg_list(t_cmd_arg *arg_list);
void					free_redir_list(t_redir *redir_list);
void					free_cmd_list(t_cmd *command_list);

//=========================UTILS====================//

char					**ft_split(char const *s, char c);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
char					*ft_strdup(const char *src);
char					*ft_strjoin(char const *s1, char const *s2);
size_t					ft_strlen(const char *s);
int						ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
