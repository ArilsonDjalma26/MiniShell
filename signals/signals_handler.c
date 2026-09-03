/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 12:00:00 by aalbano           #+#    #+#             */
/*   Updated: 2026/03/12 18:19:41 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sigint_interactive(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigint_heredoc(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	handle_sigint_exec(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}
