/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:04:12 by makamins          #+#    #+#             */
/*   Updated: 2025/08/06 17:52:08 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

t_env	*find_env_key(t_env *env, const char *key)
{
	if (!key)
		return (NULL);
	while (env)
	{
		if (env->key && ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

t_env	*create_env_node(const char *key, const char *value, t_garbage **gc)
{
	t_env	*node;

	if (!key || !gc)
		return (NULL);
	
	node = gc_malloc(sizeof(t_env), gc);
	if (!node)
		return (NULL);
	
	node->key = gc_malloc(ft_strlen(key) + 1, gc);
	if (!node->key)
		return (NULL);
	ft_strlcpy(node->key, key, ft_strlen(key) + 1);
	
	if (value)
	{
		node->value = gc_malloc(ft_strlen(value) + 1, gc);
		if (!node->value)
			return (NULL);
		ft_strlcpy(node->value, value, ft_strlen(value) + 1);
	}
	else
		node->value = NULL;
	
	node->next = NULL;
	return (node);
}

void	update_env_value(t_env *found, const char *value, t_garbage **gc)
{
	if (!found || !gc)
		return ;
		
	if (value)
	{
		found->value = gc_malloc(ft_strlen(value) + 1, gc);
		if (found->value)
			ft_strlcpy(found->value, value, ft_strlen(value) + 1);
	}
	else
		found->value = NULL;
}

void	append_env_node(t_env **env, t_env *new_node)
{
	t_env	*last;

	if (!env || !new_node)
		return ;
		
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new_node;
}

void	set_env_value(t_env **env, const char *key, const char *value, t_garbage **gc)
{
	t_env	*found;
	t_env	*new_node;

	if (!env || !key || !gc)
		return ;
	
	found = find_env_key(*env, key);
	if (found)
	{
		// Atualiza o valor da variável existente
		update_env_value(found, value, gc);
	}
	else
	{
		// Cria uma nova variável
		new_node = create_env_node(key, value, gc);
		if (new_node)
			append_env_node(env, new_node);
	}
}
