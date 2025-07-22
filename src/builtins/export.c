/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:02:10 by makamins          #+#    #+#             */
/*   Updated: 2025/07/21 21:12:02 by sabsanto         ###   ########.fr       */
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
		if (!(ft_isalnum(key[i]) || key[i] != '_'))
				return (false);	
		i++;
	}
	return (true);
}

void	print_sorted_env(t_env *env, t_minishell *mini)
{
	t_env	*sorted_list;
	t_env	*curr;
	t_env	*node_copy;
	
	if (!env)
		return ;
	sorted_list = NULL;
	curr = env;
	while (curr)
	{
		node_copy = copy_env_node(curr, &mini->gc);
		insert_sorted_env_node(&sorted_list, node_copy);
		curr = curr->next;
	}
	curr = sorted_list;
	while (curr)
	{
		printf("declare -x %s", curr->key);
		if (curr->value != NULL)
			printf("=\"%s\"", curr->value);
		printf("\n");
		curr = curr->next;
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
