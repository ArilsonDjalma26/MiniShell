/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 18:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/27 15:25:16 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	redir_in(char *file)
{
	int		fd;
	char	*error_message;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		error_message = ft_strjoin("minishell: ", file);
		perror(error_message);
		free(error_message);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	redir_out(char *file)
{
	int		fd;
	char	*error_message;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		error_message = ft_strjoin("minishell: ", file);
		perror(error_message);
		free(error_message);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	redir_append(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	redir_heredoc(char *content)
{
	int		fd_list[2];
	size_t	len;

	if (pipe(fd_list) < 0)
		return (-1);
	len = ft_strlen(content);
	write(fd_list[1], content, len);
	close(fd_list[1]);
	if (dup2(fd_list[0], STDIN_FILENO) < 0)
	{
		close(fd_list[0]);
		return (-1);
	}
	close(fd_list[0]);
	return (0);
}

int	apply_redirections(t_redir *redirs)
{
	t_redir	*current_redir;
	int		status;

	current_redir = redirs;
	while (current_redir)
	{
		status = 0;
		if (current_redir->type == TOKEN_REDIR_IN)
			status = redir_in(current_redir->file);
		else if (current_redir->type == TOKEN_REDIR_OUT)
			status = redir_out(current_redir->file);
		else if (current_redir->type == TOKEN_APPEND)
			status = redir_append(current_redir->file);
		else if (current_redir->type == TOKEN_HEREDOC)
			status = redir_heredoc(current_redir->file);
		if (status < 0)
			return (-1);
		current_redir = current_redir->next;
	}
	return (0);
}
