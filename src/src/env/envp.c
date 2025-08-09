/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:25:19 by makamins          #+#    #+#             */
/*   Updated: 2025/08/07 19:50:09 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

static int	process_env_entry(char *env_str, t_minishell *mini)
{
	char	*eq_pos;
	char	*key;
	char	*value;

	eq_pos = ft_strchr(env_str, '=');
	if (!eq_pos)
		return (1);
	key = gc_malloc(eq_pos - env_str + 1, &mini->gc_persistent);
	if (!key)
		return (0);
	ft_strlcpy(key, env_str, eq_pos - env_str + 1);
	value = gc_malloc(ft_strlen(eq_pos + 1) + 1, &mini->gc_persistent);
	if (!value)
		return (0);
	ft_strlcpy(value, eq_pos + 1, ft_strlen(eq_pos + 1) + 1);
	set_env_value(&mini->env, key, value, &mini->gc_persistent);
	return (1);
}

void	init_env_list(t_minishell *mini, char **envp)
{
	int	i;

	if (!mini || !envp)
		return ;
	mini->env = NULL;
	i = 0;
	while (envp[i])
	{
		if (!process_env_entry(envp[i], mini))
			return ;
		i++;
	}
	update_shlvl(mini);
}

static char	*make_env_entry(t_env *node, t_garbage **gc)
{
	char	*entry;
	size_t	key_len;
	size_t	val_len;
	size_t	total_len;

	if (!node || !node->key)
		return (NULL);
	key_len = ft_strlen(node->key);
	val_len = 0;
	if (node->value)
		val_len = ft_strlen(node->value);
	total_len = key_len + 1 + val_len + 1;
	entry = gc_malloc(total_len, gc);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, node->key, total_len);
	ft_strlcat(entry, "=", total_len);
	if (node->value)
		ft_strlcat(entry, node->value, total_len);
	return (entry);
}

static int	count_env_with_values(t_env *env)
{
	t_env	*curr;
	int		count;

	count = 0;
	curr = env;
	while (curr)
	{
		if (curr->value)
			count++;
		curr = curr->next;
	}
	return (count);
}

char	**env_list_to_array(t_env *env, t_garbage **gc)
{
	char	**array;
	t_env	*curr;
	int		count;
	int		i;

	count = count_env_with_values(env);
	array = gc_malloc(sizeof(char *) * (count + 1), gc);
	if (!array)
		return (NULL);
	i = 0;
	curr = env;
	while (curr)
	{
		if (curr->value)
		{
			array[i] = make_env_entry(curr, gc);
			if (!array[i])
				return (NULL);
			i++;
		}
		curr = curr->next;
	}
	array[i] = NULL;
	return (array);
}
