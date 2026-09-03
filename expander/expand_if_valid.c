/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_if_valid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/12 20:11:12 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	join_expanded(char **fs, char *expanded)
{
	char	*tmp;

	tmp = *fs;
	*fs = ft_strjoin(*fs, expanded);
	free(tmp);
	free(expanded);
}

static void	expand_special(t_token *token, int *i, char **fs, t_shell *shell)
{
	char	*expanded;

	if (token->value[*i + 1] == '0')
	{
		join_expanded(fs, ft_strdup("minishell"));
		*i += 2;
	}
	else if (token->value[*i + 1] == '@')
		*i += 2;
	else if (ft_isdigit(token->value[*i + 1]))
		*i += 2;
	else if (token->value[*i + 1] == '?')
	{
		expanded = ft_itoa(shell->last_exit);
		join_expanded(fs, expanded);
		*i += 2;
	}
}

static void	expand_env_var(t_token *token, int *i, char **fs, t_shell *shell)
{
	int		end;
	char	*key;
	char	*expanded;

	end = 0;
	while (ft_isalnum(token->value[*i + 1 + end])
		|| token->value[*i + 1 + end] == '_')
		end++;
	key = ft_substr(token->value, (unsigned int)(*i + 1), end);
	expanded = expand_env(key, shell->envs);
	free(key);
	join_expanded(fs, expanded);
	*i += 1 + end;
}

void	expand_if_valid(t_token *token, int *i, char **fs, t_shell *shell)
{
	if (token->value[*i + 1] == '0'
		|| ft_isdigit(token->value[*i + 1])
		|| token->value[*i + 1] == '@'
		|| token->value[*i + 1] == '?')
		expand_special(token, i, fs, shell);
	else if (ft_isalnum(token->value[*i + 1])
		|| token->value[*i + 1] == '_')
		expand_env_var(token, i, fs, shell);
	else
	{
		*fs = append_chr(*fs, '$');
		(*i)++;
	}
}
