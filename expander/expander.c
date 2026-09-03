/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:51:43 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:55 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_quote_char(char c, int state)
{
	if (c == '\'' && state != 2)
		return (1);
	if (c == '"' && state != 1)
		return (1);
	return (0);
}

static void	expand_loop(t_token *token, char **fs, int *state, t_shell *shell)
{
	int	i;

	i = 0;
	while (token->value[i])
	{
		if (is_quote_char(token->value[i], *state))
		{
			toggle_quote_state(state, token, i);
			i++;
		}
		else if (token->value[i] == '$' && *state != 1)
			expand_if_valid(token, &i, fs, shell);
		else
			no_expand(token, &i, fs);
	}
}

char	*expand_word(t_token *token, t_shell **shell)
{
	char	*final_string;
	int		state;

	state = 0;
	final_string = ft_strdup("");
	expand_loop(token, &final_string, &state, *shell);
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
			return (ft_strdup(current_env->value));
		current_env = current_env->next;
	}
	return (ft_strdup(""));
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
