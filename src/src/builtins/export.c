/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:02:10 by makamins          #+#    #+#             */
/*   Updated: 2025/08/07 19:41:25 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

static t_env	*create_sorted_env_list(t_env *env, t_minishell *mini)
{
	t_env	*sorted_list;
	t_env	*curr;
	t_env	*node_copy;

	if (!env || !mini)
		return (NULL);
	sorted_list = NULL;
	curr = env;
	while (curr)
	{
		if (curr->key)
		{
			node_copy = copy_env_node(curr, &mini->gc_temp);
			if (node_copy && node_copy->key)
				insert_sorted_env_node(&sorted_list, node_copy);
		}
		curr = curr->next;
	}
	return (sorted_list);
}

static void	print_sorted_env(t_env *env, t_minishell *mini)
{
	t_env	*sorted_list;
	t_env	*curr;

	sorted_list = create_sorted_env_list(env, mini);
	curr = sorted_list;
	while (curr)
	{
		printf("declare -x %s", curr->key);
		if (curr->value)
			printf("=\"%s\"", curr->value);
		printf("\n");
		curr = curr->next;
	}
}

static int	process_export_key(char *arg, t_minishell *mini)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	key = gc_malloc(i + 1, &mini->gc_temp);
	if (!key)
		return (1);
	ft_strlcpy(key, arg, i + 1);
	if (!is_valid_id_export(key))
	{
		print_export_error(key);
		return (1);
	}
	if (arg[i] == '=')
		value = arg + i + 1;
	else
		value = NULL;
	if (set_env_value(&mini->env, key, value, &mini->gc_persistent) != 0)
		return (1);
	return (0);
}

static int	export_variable(char *arg, t_minishell *mini)
{
	if (!arg || !mini)
		return (1);
	if (ft_strchr(arg, '=') == NULL)
	{
		if (!is_valid_id_export(arg))
		{
			print_export_error(arg);
			return (1);
		}
		set_env_value(&mini->env, arg, NULL, &mini->gc_persistent);
		return (0);
	}
	return (process_export_key(arg, mini));
}

int	ft_export(char **argv, t_minishell *mini)
{
	int	i;
	int	status;

	if (!argv || !mini)
		return (1);
	if (!argv[1])
	{
		print_sorted_env(mini->env, mini);
		return (0);
	}
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (export_variable(argv[i], mini) != 0)
			status = 1;
		i++;
	}
	return (status);
}
