/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:02:10 by makamins          #+#    #+#             */
/*   Updated: 2025/08/06 19:52:25 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	print_export_error(const char *arg)
{
	write(2, "export: `", 9);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 27);
}

/*bool	is_valid_id_export(const char *key)
{
	int	i;

	if (!key || key[0] == '\0')
		return (false);
	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (false);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}*/

static void	print_sorted_env(t_env *env, t_minishell *mini)
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
			node_copy = copy_env_node(curr, &mini->gc_temp);
			if (node_copy && node_copy->key)
				insert_sorted_env_node(&sorted_list, node_copy);
		}
		curr = curr->next;
	}
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
		print_export_error(arg);
		return (1);
	}
	
	if (arg[i] == '=')
		value = arg + i + 1;
	else
		value = NULL;
	
	set_env_value(&mini->env, key, value, &mini->gc_persistent);
	return (0);
}

static int	export_variable(char *arg, t_minishell *mini)
{
	if (!arg || !mini)
		return (1);
	
	// Se não tem '=', só adiciona a variável sem valor
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
	
	// Se não tem argumentos, mostra todas as variáveis
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
