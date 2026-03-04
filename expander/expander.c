#include "../minishell.h"

t_env	*initialize_envs(char **envp)
{
	int		x;
	int		start;
	int		end;
	t_env	*envs;

	envs = NULL;
	x = 0;
	while (envp[x])
	{
		start = 0;
		end = 0;
		while (envp[x][start] != '=')
			start++;
		while (envp[x][start + end])
			end++;
		push_env(&envs, ft_substr(envp[x], 0, start), ft_substr(envp[x], start
				+ 1, end));
		x++;
	}
	return (envs);
}

char	*expand_word(t_token *token, t_shell **shell)
{
	char	*final_string;
	int		state;
	int		i;

	state = 0;
	i = 0;
	state = 0;
	final_string = ft_strdup("");
	while (token->value[i])
	{
		toggle_quote_state(&state, token, i);
		if (token->value[i] == '$' && state != 1)
		{
			expand_if_valid(token, &i, &final_string, *shell);
		}
		else
			no_expand(token, &i, &final_string);
	}
	if (state != 0)
	{
		free(final_string);
		print_error("syntax error: unclosed quote");
		(*shell)->last_exit = 2;
		return (NULL);
	}
	return (final_string);
}

char	*expand_env(char *env, t_env *envs)
{
	t_env	*current_env;

	current_env = envs;
	while (current_env)
	{
		if (ft_strcmp(env, current_env->key) == 0)
		{
			return (ft_strdup(current_env->value));
		}
		current_env = current_env->next;
	}
	return (ft_strdup(""));
}

static char	*expand_dollar(char *str, int *i, t_shell *shell)
{
	int		end;
	char	*key;
	char	*value;

	if (str[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(shell->last_exit));
	}
	if (str[*i + 1] == '0')
	{
		*i += 2;
		return (ft_strdup("minishell"));
	}
	if (ft_isdigit(str[*i + 1]))
	{
		*i += 2;
		return (ft_strdup(""));
	}
	if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
	{
		end = 0;
		while (ft_isalnum(str[*i + 1 + end]) || str[*i + 1 + end] == '_')
			end++;
		key = ft_substr(str, *i + 1, end);
		value = expand_env(key, shell->envs);
		free(key);
		*i += 1 + end;
		return (value);
	}
	(*i)++;
	return (ft_strdup("$"));
}

char	*expand_heredoc_str(char *str, t_shell *shell)
{
	char	*result;
	char	*expanded;
	char	*tmp;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' '
			&& str[i + 1] != '\n')
		{
			expanded = expand_dollar(str, &i, shell);
			tmp = result;
			result = ft_strjoin(result, expanded);
			free(tmp);
			free(expanded);
		}
		else
		{
			result = append_chr(result, str[i]);
			i++;
		}
	}
	return (result);
}

void	print_envs(t_env *envs)
{
	t_env	*current;

	current = envs;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}
