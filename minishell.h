/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 12:44:53 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/12 19:48:26 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>

/* ========================= ENUMS ========================= */

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

/* ========================= STRUCTS ========================= */

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

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_shell
{
	t_token				*tokens;
	t_cmd				*commands;
	t_env				*envs;
	int					last_exit;
}						t_shell;

typedef struct s_pipe_ctx
{
	int					**pipes;
	int					n;
	int					i;
}						t_pipe_ctx;

/* ========================= Validators ========================= */

void					validate_args(int ac, char **av, char **envp);
void					close_shell(t_shell **shell, int exit_code);

/* ========================= Initializers ========================= */

void					initialize_shell(t_shell **shell, char **envp);
t_env					*initialize_envs(char **envp);

/* ========================= LEXER ========================= */

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
char					*extract_quoted(char *input, int *i,
							t_quote_type *quote_type);
char					*extract_env(char *input, int *i);
char					*extract_plain(char *input, int *i);
int						is_word_char(char c);
char					*join_and_free(char *result, char *part);
void					free_tokens(t_token *list);

/* ========================= PARSER ========================= */

void					push_arg(t_cmd_arg **head, char *new_value);
void					print_error(char *message);
void					push_redir(t_redir **head, t_token_type type,
							char *file);
t_cmd					*create_cmd(void);
t_cmd					*parser(t_token *token, t_shell **shell);
void					print_cmds(t_cmd *cmd_list);
void					free_arg_list(t_cmd_arg *arg_list);
void					free_redir_list(t_redir *redir_list);
void					free_cmd_list(t_cmd *command_list);
void					free_env_list(t_env *env_list);
int						parse_words(t_token **current_token, int *i,
							t_cmd **command, t_shell **shell);
int						parse_redirs(t_token **current_token,
							t_cmd **command_list, t_cmd **command,
							t_shell **shell);
int						parse_pipe(t_token **current_token, t_cmd **command,
							t_cmd **command_list, t_shell **shell);
char					*read_heredoc(char *delimiter);

/* ========================= EXPANDER ========================= */

void					push_env(t_env **head, char *key, char *value);
void					print_envs(t_env *envs);
char					*expand_env(char *env, t_env *envs);
char					*expand_word(t_token *token, t_shell **shell);
char					*expand_heredoc_str(char *str, t_shell *shell);
char					*append_chr(char *s, char c);
char					*remove_quotes(char *str);
void					toggle_quote_state(int *state, t_token *token, int i);
void					expand_if_valid(t_token *token, int *i,
							char **final_string, t_shell *shell);
void					no_expand(t_token *token, int *i, char **final_string);

/* ========================= EXECUTOR ========================= */

int						count_cmds(t_cmd *cmd);
int						execute(t_shell **shell);
int						exec_pipeline(t_shell **shell, int n);
int						**create_pipes(int n);
void					close_pipes(int **pipes, int n);
void					cleanup_pipes(int **pipes, int i);
int						wait_all(pid_t *pids, int n, pid_t last_pid);
void					exec_child(t_cmd *cmd, t_pipe_ctx *ctx,
							t_shell **shell);
void					child_close_pipes(int **pipes, int n);
void					child_setup_fds(t_pipe_ctx *ctx);
void					child_error_exit(char *path, char **argv, t_shell **sh,
							int code);
void					child_check_path(char *path, char **argv,
							t_shell **shell);
char					*find_cmd_path(char *cmd, t_env *envs);
int						apply_redirections(t_redir *redirs);
int						is_builtin(char *cmd);
int						exec_builtin(t_cmd *cmd, t_shell **shell);
char					**cmd_args_to_array(t_cmd *cmd);
char					**env_to_array(t_env *envs);

/* ========================= BUILT-INS ========================= */

int						builtin_echo(t_cmd *cmd);
int						builtin_cd(t_cmd *cmd, t_shell **shell);
int						builtin_pwd(void);
int						builtin_env(t_shell *shell);
int						builtin_export(t_cmd *cmd, t_shell **shell);
int						builtin_unset(t_cmd *cmd, t_shell **shell);
int						builtin_exit(t_cmd *cmd, t_shell **shell);
int						is_valid_identifier(char *arg);
t_env					*sort_list(t_env *envs);
void					update_env(t_shell **shell, char *key, char *value);
int						env_exists(t_shell **shell, char *key);
void					print_export_list(t_shell **shell);

/* ========================= SIGNALS ========================= */

extern int				g_signal_received;
void					setup_signals_interactive(void);
void					setup_signals_exec(void);
void					setup_signals_child(void);
void					setup_signals_heredoc(void);
void					handle_sigint_interactive(int sig);
void					handle_sigint_heredoc(int sig);
void					handle_sigint_exec(int sig);

/* ========================= UTILS ========================= */

char					**ft_split(char const *s, char c);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
char					*ft_strdup(const char *src);
char					*ft_strjoin(char const *s1, char const *s2);
size_t					ft_strlen(const char *s);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_strcmp(char *s1, char *s2);
char					*ft_strchr(const char *s, int c);
int						ft_isalnum(int c);
void					ft_free_split(char **result);
int						ft_isalpha(int c);
int						ft_isdigit(int c);
char					*ft_itoa(int n);
int						ft_atoi(const char *nptr, t_shell **shell);

#endif
