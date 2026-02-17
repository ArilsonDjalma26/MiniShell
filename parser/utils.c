/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:51:57 by elfranco          #+#    #+#             */
/*   Updated: 2026/02/13 09:51:58 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	push_arg(t_cmd_arg **head, char *new_value)
{
	t_cmd_arg	*new_arg;

	new_arg = (t_cmd_arg *)malloc(sizeof(t_cmd_arg));
	if (!new_arg)
		return ;
	new_arg->value = ft_strdup(new_value);
	new_arg->next = NULL;
	while (*head)
		head = &(*head)->next;
	*head = new_arg;
}

void	push_redir(t_redir **head, t_token_type type, char *file)
{
	t_redir	*new_redir;

	new_redir = (t_redir *)malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->type = type;
	new_redir->file = ft_strdup(file);
	new_redir->next = NULL;
	while (*head)
		head = &(*head)->next;
	*head = new_redir;
}

t_cmd	*create_cmd(void)
{
	t_cmd	*command;

	command = (t_cmd *)malloc(sizeof(t_cmd));
	if (!command)
		return (NULL);
	command->args = NULL;
	command->redirs = NULL;
	command->argc = 0;
	command->next = NULL;
	return (command);
}

void	print_error(char *message)
{
	fprintf(stderr, "minishell: %s\n", message);
}