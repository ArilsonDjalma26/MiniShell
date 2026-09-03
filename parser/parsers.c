/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 11:52:45 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	push_split_args(char *expanded, t_cmd **command)
{
	char	**splited;
	int		i;

	i = 0;
	splited = ft_split(expanded, ' ');
	free(expanded);
	if (!splited)
		return ;
	while (splited[i])
	{
		push_arg(&(*command)->args, splited[i]);
		(*command)->argc++;
		i++;
	}
	ft_free_split(splited);
}

int	parse_words(t_token **cur, int *i, t_cmd **command, t_shell **shell)
{
	char	*expanded;

	(void)i;
	expanded = expand_word(*cur, shell);
	if (!expanded)
		return (2);
	if ((*cur)->quote == QUOTE_NONE)
		push_split_args(expanded, command);
	else
	{
		push_arg(&(*command)->args, expanded);
		free(expanded);
		(*command)->argc++;
	}
	*cur = (*cur)->next;
	return (0);
}

int	parse_pipe(t_token **cur, t_cmd **command,
		t_cmd **command_list, t_shell **shell)
{
	if (((*command)->argc == 0 && !(*command)->redirs) || !(*cur)->next)
	{
		free_cmd_list(*command_list);
		*command_list = NULL;
		print_error("syntax error near unexpected token `|'");
		(*shell)->last_exit = 2;
		return (2);
	}
	(*command)->next = create_cmd();
	*command = (*command)->next;
	*cur = (*cur)->next;
	return (0);
}
