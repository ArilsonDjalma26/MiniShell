/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_export_list(t_shell **shell)
{
	t_env	*cur;

	cur = sort_list((*shell)->envs);
	while (cur)
	{
		if (cur->value != NULL)
			printf("declare -x %s=\"%s\"\n", cur->key, cur->value);
		else
			printf("declare -x %s\n", cur->key);
		cur = cur->next;
	}
}
