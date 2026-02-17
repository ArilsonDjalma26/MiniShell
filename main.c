/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:27:27 by aalbano           #+#    #+#             */
/*   Updated: 2026/02/13 09:50:31 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*commands;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
		{
			add_history(line);
			tokens = lexer(line);
			commands = parser(tokens);
			print_cmds(commands);
			free_cmd_list(commands);
			free_tokens(tokens);
		}
		free(line);
	}
	return (0);
}
