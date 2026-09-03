/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*child_resolve_path(char **argv, t_shell **shell)
{
	char	*path;

	if (ft_strchr(argv[0], '/'))
		path = ft_strdup(argv[0]);
	else
		path = find_cmd_path(argv[0], (*shell)->envs);
	if (!path)
	{
		write(STDERR_FILENO, argv[0], ft_strlen(argv[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		ft_free_split(argv);
		close_shell(shell, 127);
	}
	return (path);
}

static void	child_exec(char *path, char **argv, t_shell **shell)
{
	char	**envp;

	child_check_path(path, argv, shell);
	envp = env_to_array((*shell)->envs);
	if (!envp)
		child_error_exit(path, argv, shell, 1);
	execve(path, argv, envp);
	perror(path);
	ft_free_split(envp);
	child_error_exit(path, argv, shell, 1);
}

void	exec_child(t_cmd *cmd, t_pipe_ctx *ctx, t_shell **shell)
{
	char	**argv;
	char	*path;

	child_setup_fds(ctx);
	if (apply_redirections(cmd->redirs) < 0)
		close_shell(shell, 1);
	if (!cmd->args)
		close_shell(shell, 0);
	if (is_builtin(cmd->args->value))
		close_shell(shell, exec_builtin(cmd, shell));
	argv = cmd_args_to_array(cmd);
	if (!argv)
		close_shell(shell, 1);
	path = child_resolve_path(argv, shell);
	child_exec(path, argv, shell);
}
