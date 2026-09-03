/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsers_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*join_heredoc_line(char *content, char *line)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(content, line);
	free(content);
	result = ft_strjoin(tmp, "\n");
	free(tmp);
	return (result);
}

static char	*heredoc_sigint(int saved_stdin, char *content)
{
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	free(content);
	setup_signals_interactive();
	return (NULL);
}

static int	heredoc_line(char **content, char *delim, int saved)
{
	char	*line;

	line = readline("> ");
	if (!line)
	{
		if (g_signal_received == SIGINT)
		{
			*content = heredoc_sigint(saved, *content);
			return (-1);
		}
		write(STDERR_FILENO,
			"minishell: warning: here-document delimited by end-of-file\n",
			59);
		return (1);
	}
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (1);
	}
	*content = join_heredoc_line(*content, line);
	free(line);
	return (0);
}

char	*read_heredoc(char *delimiter)
{
	char	*content;
	int		saved_stdin;
	int		ret;

	saved_stdin = dup(STDIN_FILENO);
	setup_signals_heredoc();
	content = ft_strdup("");
	while (1)
	{
		ret = heredoc_line(&content, delimiter, saved_stdin);
		if (ret != 0)
			break ;
	}
	if (!content)
		return (NULL);
	close(saved_stdin);
	setup_signals_interactive();
	return (content);
}
