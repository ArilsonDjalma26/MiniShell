/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_close_pipes(int **pipes, int n)
{
	int	j;

	j = 0;
	while (j < n - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		free(pipes[j]);
		j++;
	}
	free(pipes);
}

void	child_setup_fds(t_pipe_ctx *ctx)
{
	setup_signals_child();
	if (ctx->i > 0)
		dup2(ctx->pipes[ctx->i - 1][0], STDIN_FILENO);
	if (ctx->i < ctx->n - 1)
		dup2(ctx->pipes[ctx->i][1], STDOUT_FILENO);
	child_close_pipes(ctx->pipes, ctx->n);
}

void	child_error_exit(char *path, char **argv, t_shell **sh, int code)
{
	free(path);
	ft_free_split(argv);
	close_shell(sh, code);
}

static void	child_path_err(char *path, char **av, t_shell **sh, char *suffix)
{
	char	*msg;

	msg = ft_strjoin(path, suffix);
	print_error(msg);
	free(msg);
	if (ft_strcmp(suffix, ": Is a directory") == 0
		|| ft_strcmp(suffix, ": Permission denied") == 0)
		child_error_exit(path, av, sh, 126);
	else
		child_error_exit(path, av, sh, 127);
}

void	child_check_path(char *path, char **argv, t_shell **shell)
{
	struct stat	info;
	char		*msg;
	char		*tmp;

	if (stat(path, &info) == -1)
	{
		msg = ft_strjoin(path, ": ");
		tmp = ft_strjoin(msg, strerror(errno));
		free(msg);
		print_error(tmp);
		free(tmp);
		child_error_exit(path, argv, shell, 127);
	}
	if (S_ISDIR(info.st_mode))
		child_path_err(path, argv, shell, ": Is a directory");
	if (!S_ISREG(info.st_mode) || access(path, X_OK))
		child_path_err(path, argv, shell, ": Permission denied");
}
