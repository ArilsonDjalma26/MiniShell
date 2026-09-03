/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	fork_children(t_shell **sh, int **pipes, pid_t *pids, int n)
{
	t_cmd		*cur;
	int			i;
	t_pipe_ctx	ctx;

	cur = (*sh)->commands;
	i = 0;
	ctx.pipes = pipes;
	ctx.n = n;
	while (cur)
	{
		ctx.i = i;
		pids[i] = fork();
		if (pids[i] < 0)
			return (perror("fork"), -1);
		if (pids[i] == 0)
		{
			free(pids);
			exec_child(cur, &ctx, sh);
		}
		cur = cur->next;
		i++;
	}
	return (0);
}

static int	run_pipeline(t_shell **shell, int **pipes, int n)
{
	pid_t	*pids;
	int		ret;

	pids = (pid_t *)malloc(sizeof(pid_t) * n);
	if (!pids)
	{
		close_pipes(pipes, n);
		return (1);
	}
	setup_signals_exec();
	if (fork_children(shell, pipes, pids, n) < 0)
	{
		close_pipes(pipes, n);
		free(pids);
		return (1);
	}
	close_pipes(pipes, n);
	ret = wait_all(pids, n, pids[n - 1]);
	free(pids);
	setup_signals_interactive();
	return (ret);
}

int	exec_pipeline(t_shell **shell, int n)
{
	int	**pipes;

	pipes = create_pipes(n);
	if (n > 1 && !pipes)
		return (1);
	return (run_pipeline(shell, pipes, n));
}

static int	exec_single_builtin(t_shell **shell, t_cmd *cmd)
{
	int	std_in;
	int	std_out;
	int	exit_code;

	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	if (apply_redirections(cmd->redirs) < 0)
	{
		dup2(std_in, STDIN_FILENO);
		dup2(std_out, STDOUT_FILENO);
		close(std_in);
		close(std_out);
		(*shell)->last_exit = 1;
		return (1);
	}
	exit_code = exec_builtin(cmd, shell);
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
	close(std_in);
	close(std_out);
	(*shell)->last_exit = exit_code;
	return (exit_code);
}

int	execute(t_shell **shell)
{
	int		cmds_len;
	t_cmd	*cmd;
	int		exit_code;

	if (!shell || !(*shell)->commands)
		return (0);
	cmds_len = count_cmds((*shell)->commands);
	cmd = (*shell)->commands;
	if (cmds_len == 1 && cmd->args && is_builtin(cmd->args->value))
		return (exec_single_builtin(shell, cmd));
	exit_code = exec_pipeline(shell, cmds_len);
	(*shell)->last_exit = exit_code;
	return (exit_code);
}
