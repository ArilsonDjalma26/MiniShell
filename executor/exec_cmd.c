/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 18:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/03 10:17:31 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**cmd_args_to_array(t_cmd *cmd)
{
	char		**arr;
	t_cmd_arg	*cur;
	int			i;

	arr = (char **)malloc(sizeof(char *) * (cmd->argc + 1));
	if (!arr)
		return (NULL);
	cur = cmd->args;
	i = 0;
	while (cur)
	{
		arr[i] = ft_strdup(cur->value);
		if (!arr[i])
		{
			while (i > 0)
				free(arr[--i]);
			free(arr);
			return (NULL);
		}
		cur = cur->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

char	**env_to_array(t_env *envs)
{
	t_env	*cur;
	char	**arr;
	char	*tmp;
	int		count;
	int		i;

	count = 0;
	cur = envs;
	while (cur && ++count)
		cur = cur->next;
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	cur = envs;
	i = 0;
	while (cur)
	{
		tmp = ft_strjoin(cur->key, "=");
		arr[i] = ft_strjoin(tmp, cur->value);
		free(tmp);
		if (!arr[i])
		{
			while (i > 0)
				free(arr[--i]);
			free(arr);
			return (NULL);
		}
		cur = cur->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

int	exec_builtin(t_cmd *cmd, t_shell **shell)
{
	char	*name;

	if (cmd->args)
		name = cmd->args->value;
	else
		return (1);
	if (ft_strcmp(name, "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(name, "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(name, "env") == 0)
		return (builtin_env(*shell));
	if (ft_strcmp(name, "cd") == 0)
		return (builtin_cd(cmd, shell));
	if (ft_strcmp(name, "exit") == 0)
		return (builtin_exit(cmd, shell));
	if (ft_strcmp(name, "export") == 0)
		return (builtin_export(cmd, shell));
	if (ft_strcmp(name, "unset") == 0)
		return (builtin_unset(cmd, shell));
	return (1);
}

void	exec_child(t_cmd *cmd, int i, int **pipes, int n, t_shell **shell)
{
	char	**argv;
	char	**envp;
	char	*path;
	int		j;
	struct stat info;
	char *msg;

	setup_signals_child();
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < n - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	j = 0;
	while (j < n - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
	if (apply_redirections(cmd->redirs) < 0)
		exit(1);
	if (!cmd->args)
		exit(0);
	if (is_builtin(cmd->args->value))
		exit(exec_builtin(cmd, shell));
	argv = cmd_args_to_array(cmd);
	if (!argv)
		exit(1);

	if (ft_strchr(argv[0], '/'))
		path = ft_strdup(argv[0]);
	else
		path = find_cmd_path(argv[0], (*shell)->envs);
	if (!path)
	{
		
		write(STDERR_FILENO, argv[0], ft_strlen(argv[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		ft_free_split(argv);
		exit(127);
	}
	if (stat(path, &info) == -1)
	{
		msg = ft_strjoin(path, ": ");
		msg = ft_strjoin(msg, strerror(errno));
        print_error(msg);
        exit (127);
	}
	if (S_ISDIR(info.st_mode))
	{
		msg = ft_strjoin(path, ": Is a directory");
        print_error(msg);
		free(msg);
		exit (126);
	}
	if (!S_ISREG(info.st_mode) || access(path, X_OK))
	{
		msg = ft_strjoin(path, ": Permission denied");
		print_error(msg);
		free(msg);
		exit(126);
	}
	envp = env_to_array((*shell)->envs);
	if (!envp)
	{
		free(path);
		ft_free_split(argv);
		exit(1);
	}
	execve(path, argv, envp);
	perror(path);
	free(path);
	ft_free_split(argv);
	ft_free_split(envp);
	exit(1);
}
