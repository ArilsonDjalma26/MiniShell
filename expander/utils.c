#include "../minishell.h"

void	push_env(t_env **head, char *key, char *value)
{
	t_env	*new_env;

	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->value = ft_strdup(value);
	new_env->key = ft_strdup(key);
	new_env->next = NULL;
	while (*head)
		head = &(*head)->next;
	*head = new_env;
}

char	*append_chr(char *s, char c)
{
	char	new[2];
	char	*tmp;

	new[0] = c;
	new[1] = '\0';
	tmp = ft_strjoin(s, new);
	free(s);
	return (tmp);
}

char	*remove_quotes(char *str)
{
	char	*new_str;
	int		i;
	int		j;
	int		state;

	i = 0;
	j = 0;
	state = 0;
	if (!str)
		return (NULL);
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && state != 2)
		{
			state = (state == 1) ? 0 : 1;
			i++;
		}
		else if (str[i] == '"' && state != 1)
		{
			state = (state == 2) ? 0 : 2;
			i++;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}
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

void	expand_if_valid(t_token *token, int *i, char **final_string,
		t_shell *shell)
{
	int		end;
	char	*current_env;

	end = 0;
	if (token->value[*i + 1] == '0')
	{
		*final_string = ft_strjoin(*final_string, "minishell");
		*i = (*i) + 2;
	}
	else if (ft_isdigit(token->value[*i + 1]))
		*i += 2;
	else if (token->value[*i + 1] == '?')
	{
		*final_string = ft_strjoin(*final_string, ft_itoa(shell->last_exit));
		*i += 2;
	}
	else if (ft_isalnum(token->value[*i + 1]) || token->value[*i + 1] == '_')
	{
		end = 0;
		while (ft_isalnum(token->value[*i + 1 + end]) || token->value[*i + 1
			+ end] == '_')
			end++;
		current_env = ft_substr(token->value, (unsigned int)(*i + 1), end);
		*final_string = ft_strjoin(*final_string, expand_env(current_env,
					shell->envs));
		*i += 1 + end;
	}
	else
	{
		*final_string = append_chr(*final_string, '$');
		(*i)++;
	}
}

void	no_expand(t_token *token, int *i, char **final_string)
{
	*final_string = append_chr(*final_string, token->value[*i]);
	(*i)++;
}
