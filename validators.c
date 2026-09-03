/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 10:45:11 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/12 10:38:04 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	validate_args(int ac, char **av, char **envp)
{
	(void)av;
	if (ac > 1)
	{
		write(STDOUT_FILENO, "notthing expected: Success", 26);
		exit(1);
	}
	if (!envp)
	{
		write(STDOUT_FILENO, "env structure initiaization: Fail", 33);
		exit(1);
	}
}
