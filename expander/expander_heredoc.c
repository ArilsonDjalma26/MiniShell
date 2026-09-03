/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/12 20:11:12 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*dollar_special(char *str, int *i, t_shell *shell)
{
	if (str[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(shell->last_exit));
	}
	if (str[*i + 1] == '@')
	{
		*i += 2;
		return (ft_strdup(""));
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
	return (NULL);
}

static char	*dollar_env(char *str, int *i, t_shell *shell)
{
	int		end;
	char	*key;
	char	*value;

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

static char	*expand_dollar(char *str, int *i, t_shell *shell)
{
	char	*result;

	result = dollar_special(str, i, shell);
	if (result)
		return (result);
	return (dollar_env(str, i, shell));
}

static void	heredoc_expand(char *str, int *i, char **result, t_shell *shell)
{
	char	*expanded;
	char	*tmp;

	expanded = expand_dollar(str, i, shell);
	tmp = *result;
	*result = ft_strjoin(*result, expanded);
	free(tmp);
	free(expanded);
}

char	*expand_heredoc_str(char *str, t_shell *shell)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1]
			&& str[i + 1] != ' ' && str[i + 1] != '\n')
			heredoc_expand(str, &i, &result, shell);
		else
		{
			result = append_chr(result, str[i]);
			i++;
		}
	}
	return (result);
}
