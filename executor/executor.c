/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 18:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/12 17:46:32 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_pipes(int **pipes, int i)
{
	while (i > 0)
	{
		close(pipes[i - 1][0]);
		close(pipes[i - 1][1]);
		free(pipes[--i]);
	}
	free(pipes);
}

int	**create_pipes(int n)
{
	int	**pipes;
	int	i;

	if (n <= 1)
		return (NULL);
	pipes = (int **)malloc(sizeof(int *) * (n - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < n - 1)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) < 0)
		{
			cleanup_pipes(pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	close_pipes(int **pipes, int n)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < n - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	parse_signal(int *last_status, int status)
{
	if (WIFSIGNALED(status))
	{
		*last_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
}

int	wait_all(pid_t *pids, int n, pid_t last_pid)
{
	int		status;
	int		last_status;
	pid_t	wpid;

	last_status = 0;
	while (1)
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid < 0 && errno == ECHILD)
			break ;
		if (wpid == last_pid)
		{
			parse_signal(&last_status, status);
		}
	}
	(void)pids;
	(void)n;
	return (last_status);
}
