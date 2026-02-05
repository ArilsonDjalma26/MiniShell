/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <elfranco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 13:06:07 by elfranco          #+#    #+#             */
/*   Updated: 2026/01/13 13:09:20 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

static int	ft_count_words(const char *s, char c)
{
	int	in_word;
	int	in_block;
	int	counter;

	in_word = 0;
	in_block = 0;
	counter = 0;
	while (s && *s)
	{
		if (*s == '\'')
		{
			in_block = !in_block;
			if (in_block && ++counter)
				in_word = 1;
			else
				in_word = 0;
		}
		else if (*s != c && !in_word && ++counter)
			in_word = 1;
		else if (*s == c && !in_block)
			in_word = 0;
		s++;
	}
	return (counter);
}

static size_t	ft_find_end(const char *s, int start, char c, int in_block)
{
	size_t	end;

	end = 0;
	if (in_block == 1)
	{
		while (s[start + end] != '\'' && s[start + end])
			end++;
	}
	else
	{
		while (s[start + end] != c && s[start + end])
			end++;
	}
	if (in_block == 1)
		return (end);
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

static char	*allocate_word(char const *s, int *start, char c, int *in_block)
{
	char	*word;
	int		end;

	end = 0;
	while (s[*start] == c && s[*start])
		(*start)++;
	if (s[*start] == '\'')
	{
		*in_block = 1;
		end = ft_find_end(s, *start + 1, c, *in_block);
		word = ft_substr(s, *start + 1, end);
	}
	else
		word = ft_substr(s, *start, ft_find_end(s, *start, c, *in_block));
	if (word)
		*start = *start + ft_find_end(s, *start, c, *in_block);
	*in_block = 0;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		start;
	int		in_block;

	start = 0;
	in_block = 0;
	i = 0;
	result = (char **)malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (!result || !s)
		return (NULL);
	while (i < ft_count_words(s, c))
	{
		result[i] = allocate_word(s, &start, c, &in_block);
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
