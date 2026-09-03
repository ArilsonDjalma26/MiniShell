/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 13:06:07 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 13:57:17 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_words(const char *s, char c)
{
	int	in_word;
	int	counter;

	in_word = 0;
	counter = 0;
	while (s && *s)
	{
		if (*s != c && !in_word && ++counter)
			in_word = 1;
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (counter);
}

static size_t	ft_find_end(const char *s, int start, char c)
{
	size_t	end;

	end = 0;
	while (s[start + end] && s[start + end] != c)
		end++;
	return (end);
}

void	ft_free_split(char **result)
{
	int	i;

	i = 0;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
}

static char	*allocate_word(char const *s, int *start, char c)
{
	char	*word;

	while (s[*start] == c && s[*start])
		(*start)++;
	word = ft_substr(s, *start, ft_find_end(s, *start, c));
	if (word)
		*start = *start + ft_find_end(s, *start, c);
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		start;

	start = 0;
	i = 0;
	result = (char **)malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (!result || !s)
		return (NULL);
	while (i < ft_count_words(s, c))
	{
		result[i] = allocate_word(s, &start, c);
		if (!result[i])
		{
			ft_free_split(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}
