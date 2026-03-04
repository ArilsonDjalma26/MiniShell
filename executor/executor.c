/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 18:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/27 15:25:19 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	**create_pipes(int n)
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
			while (i > 0)
			{
				close(pipes[i - 1][0]);
				close(pipes[i - 1][1]);
				free(pipes[--i]);
			}
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

static void	close_pipes(int **pipes, int n)
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

static int	wait_all(pid_t *pids, int n, pid_t last_pid)
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
			if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
			else if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}
	}
	(void)pids;
	(void)n;
	return (last_status);
}

static int	exec_pipeline(t_shell **shell, int n)
{
	int		**pipes;
	pid_t	*pids;
	pid_t	pid;
	t_cmd	*cur;
	int		i;

	pipes = create_pipes(n);
	if (n > 1 && !pipes)
		return (1);
	pids = (pid_t *)malloc(sizeof(pid_t) * n);
	if (!pids)
	{
		close_pipes(pipes, n);
		return (1);
	}
	setup_signals_exec();
	cur = (*shell)->commands;
	i = 0;
	while (cur)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			close_pipes(pipes, n);
			free(pids);
			return (1);
		}
		if (pid == 0)
			exec_child(cur, i, pipes, n, shell);
		pids[i] = pid;
		cur = cur->next;
		i++;
	}
	close_pipes(pipes, n);
	i = wait_all(pids, n, pids[n - 1]);
	free(pids);
	setup_signals_interactive();
	return (i);
}

int	execute(t_shell **shell)
{
	int		cmds_len;
	t_cmd	*current_cmd;
	int		STD_IN;
	int		STD_OUT;
	int		exit_code;

	if (!shell || !(*shell)->commands)
		return (0);
	cmds_len = count_cmds((*shell)->commands);
	current_cmd = (*shell)->commands;
	if (cmds_len == 1 && current_cmd->args
		&& is_builtin(current_cmd->args->value))
	{
		STD_IN = dup(STDIN_FILENO);
		STD_OUT = dup(STDOUT_FILENO);
		if (apply_redirections(current_cmd->redirs) < 0)
		{
			dup2(STD_IN, STDIN_FILENO);
			dup2(STD_OUT, STDOUT_FILENO);
			close(STD_IN);
			close(STD_OUT);
			(*shell)->last_exit = 1;
			return (1);
		}
		exit_code = exec_builtin(current_cmd, shell);
		dup2(STD_IN, STDIN_FILENO);
		dup2(STD_OUT, STDOUT_FILENO);
		close(STD_IN);
		close(STD_OUT);
		(*shell)->last_exit = exit_code;
		return (exit_code);
	}
	exit_code = exec_pipeline(shell, cmds_len);
	(*shell)->last_exit = exit_code;
	return (exit_code);
}
