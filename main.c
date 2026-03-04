/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:27:27 by aalbano           #+#    #+#             */
/*   Updated: 2026/03/02 16:36:11 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	*shell;

	(void)argc;
	(void)argv;
	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (1);
	shell->envs = initialize_envs(envp);
	shell->last_exit = 0;
	setup_signals_interactive();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (g_signal_received)
		{
			shell->last_exit = 130;
			g_signal_received = 0;
		}
		if (*line)
		{
			add_history(line);
			shell->tokens = lexer(line);
			shell->commands = parser(shell->tokens, &shell);
			if (shell->commands)
				execute(&shell);
			free_cmd_list(shell->commands);
			free_tokens(shell->tokens);
		}
		free(line);
	}
	return (shell->last_exit);
}

