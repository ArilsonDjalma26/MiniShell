/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:27:27 by aalbano           #+#    #+#             */
/*   Updated: 2026/03/12 20:14:45 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_readline(t_shell **shell)
{
	int	exit_code;

	exit_code = (*shell)->last_exit;
	rl_clear_history();
	close_shell(shell, exit_code);
}

static int	is_blank_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || (line[i] >= 9 && line[i] <= 13))
		i++;
	return (line[i] == '\0');
}

static void	process_line(char *line, t_shell **shell)
{
	if (is_blank_line(line))
		return ;
	add_history(line);
	(*shell)->tokens = lexer(line);
	(*shell)->commands = parser((*shell)->tokens, &(*shell));
	if ((*shell)->commands)
		execute(&(*shell));
	free_cmd_list((*shell)->commands);
	(*shell)->commands = NULL;
	free_tokens((*shell)->tokens);
	(*shell)->tokens = NULL;
}

void	readline_loop(char **line, t_shell **shell)
{
	*line = readline("minishell$ ");
	if (g_signal_received)
	{
		(*shell)->last_exit = 130;
		g_signal_received = 0;
	}
	if (!(*line))
	{
		write(STDOUT_FILENO, "exit\n", 5);
		free_readline(shell);
	}
	if (*line)
		process_line(*line, shell);
	free(*line);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*line;

	validate_args(argc, argv, envp);
	shell = (t_shell *)malloc(sizeof(t_shell));
	initialize_shell(&shell, envp);
	setup_signals_interactive();
	while (1)
		readline_loop(&line, &shell);
	free_readline(&shell);
}
