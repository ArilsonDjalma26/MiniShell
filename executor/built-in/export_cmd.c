/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elfranco <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:00:00 by elfranco          #+#    #+#             */
/*   Updated: 2026/03/05 20:01:14 by elfranco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	export_error(char *arg_value)
{
	char	*msg;
	char	*tmp;

	msg = ft_strjoin("export: ", arg_value);
	tmp = ft_strjoin(msg, ": not a valid identifier");
	free(msg);
	print_error(tmp);
	free(tmp);
	return (1);
}

static void	export_parse_kv(char *str, char **key, char **value)
{
	int	start;
	int	end;

	start = 0;
	end = 0;
	while (str[start] && str[start] != '=')
		start++;
	while (str[start + end])
		end++;
	*key = ft_substr(str, 0, start);
	*value = ft_substr(str, start + 1, end);
}

static void	export_new_var(t_shell **shell, t_cmd_arg *arg, char *key,
		char *value)
{
	if (ft_strchr(arg->value, '='))
	{
		push_env(&(*shell)->envs, key, value);
		free(value);
	}
	else
	{
		push_env(&(*shell)->envs, key, NULL);
		free(value);
	}
}

static void	export_set_var(t_shell **shell, t_cmd_arg *arg)
{
	char	*key;
	char	*value;

	export_parse_kv(arg->value, &key, &value);
	if (env_exists(shell, key))
	{
		if (ft_strchr(arg->value, '='))
			update_env(shell, key, value);
		else
			free(value);
	}
	else
		export_new_var(shell, arg, key, value);
	free(key);
}

int	builtin_export(t_cmd *cmd, t_shell **shell)
{
	t_cmd_arg	*cur;

	cur = cmd->args->next;
	if (!cur)
	{
		print_export_list(shell);
		return (0);
	}
	while (cur)
	{
		if (!is_valid_identifier(cur->value))
			return (export_error(cur->value));
		export_set_var(shell, cur);
		cur = cur->next;
	}
	return (0);
}
