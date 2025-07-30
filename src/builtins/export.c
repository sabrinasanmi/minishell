/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:02:10 by makamins          #+#    #+#             */
/*   Updated: 2025/07/30 17:48:21 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

bool	is_valid_id_export(const char *key)
{
	int	i;

	if (!key || key[0] == '\0')
		return (false);
	if (!(ft_isalpha(key[0]) || key[0] != '_'))
		return (false);
	i = 0;
	while (key[i] && key[i] != '=')
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
				return (false);	
		i++;
	}
	return (true);
}

void	print_env_line(t_env *node)
{
	if (!node || !node->key)
		return ;
	printf("declare -x %s", node->key);
	if (node->value)
		printf("=\"%s\"", node->value);
	printf("\n");
}

void	print_sorted_env(t_env *env, t_minishell *mini)
{
	t_env	*sorted_list;
	t_env	*curr;
	t_env	*node_copy;
	
	if (!env || !mini)
		return ;
	sorted_list = NULL;
	curr = env;
	while (curr)
	{
		if (curr->key)
		{
			node_copy = copy_env_node(curr, &mini->gc);
			if (node_copy && node_copy->key)
				insert_sorted_env_node(&sorted_list, node_copy);
		}
		curr = curr->next;
	}
	curr = sorted_list;
	while (sorted_list)
	{
		print_env_line(sorted_list);
		sorted_list = sorted_list->next;
	}
	
}

int	process_key(char *arg, t_minishell *mini)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	key = gc_malloc(i + 1, &mini->gc);
	if (!key)
		return (1);
	ft_strlcpy(key, arg, i + 1);
	if (!is_valid_id_export(key))
	{
		print_export_error(arg);
		return (1);
	}
	if (arg[i] == '=')
		value = arg + i + 1;
	else
		value = "";
	set_env_value(&mini->env, key, value, &mini->gc);
	return (0);
}

int	export_variable(char *arg, t_minishell *mini)
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
		set_env_value(&mini->env, arg, "", &mini->gc);
	}
	else
		process_key(arg, mini);
	return (0);
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
