/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:51:43 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	push_env(t_env **head, char *key, char *value)
{
	t_env	*new_env;

	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->key = ft_strdup(key);
	if (value)
		new_env->value = ft_strdup(value);
	else
		new_env->value = NULL;
	new_env->next = NULL;
	while (*head)
		head = &(*head)->next;
	*head = new_env;
}

char	*append_chr(char *s, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(s, buf);
	free(s);
	return (tmp);
}

static void	rq_toggle(char c, int *state)
{
	if (c == '\'' && *state == 0)
		*state = 1;
	else if (c == '\'' && *state == 1)
		*state = 0;
	else if (c == '"' && *state == 0)
		*state = 2;
	else if (c == '"' && *state == 2)
		*state = 0;
}

static char	*rq_alloc(char *str)
{
	if (!str)
		return (NULL);
	return (malloc(sizeof(char) * (ft_strlen(str) + 1)));
}

char	*remove_quotes(char *str)
{
	char	*ns;
	int		ij[2];
	int		state;

	ns = rq_alloc(str);
	if (!ns)
		return (NULL);
	ij[0] = 0;
	ij[1] = 0;
	state = 0;
	while (str[ij[0]])
	{
		if ((str[ij[0]] == '\'' && state != 2)
			|| (str[ij[0]] == '"' && state != 1))
		{
			rq_toggle(str[ij[0]], &state);
			ij[0]++;
		}
		else
			ns[ij[1]++] = str[ij[0]++];
	}
	ns[ij[1]] = '\0';
	return (ns);
}
