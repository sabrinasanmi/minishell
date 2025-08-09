/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:56:34 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 14:07:41 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "minishell.h"

static int	copy_env_strings(t_env *new_node, t_env *src, t_garbage **gc)
{
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(src->key) + 1;
	new_node->key = gc_malloc(key_len, gc);
	if (!new_node->key)
		return (-1);
	ft_strlcpy(new_node->key, src->key, key_len);
	if (src->value)
	{
		value_len = ft_strlen(src->value) + 1;
		new_node->value = gc_malloc(value_len, gc);
		if (!new_node->value)
			return (-1);
		ft_strlcpy(new_node->value, src->value, value_len);
	}
	else
		new_node->value = NULL;
	return (0);
}

t_env	*copy_env_node(t_env *src, t_garbage **gc)
{
	t_env	*new_node;

	if (!src || !src->key || !gc)
		return (NULL);
	new_node = gc_malloc(sizeof(t_env), gc);
	if (!new_node)
		return (NULL);
	if (copy_env_strings(new_node, src, gc) == -1)
		return (NULL);
	new_node->next = NULL;
	return (new_node);
}

size_t	max_strlen(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	if (len1 > len2)
		return (len1);
	return (len2);
}

void	insert_sorted_env_node(t_env **sorted, t_env *new_node)
{
	t_env	*curr;
	t_env	*prev;
	size_t	n;

	if (!sorted || !new_node || !new_node->key)
		return ;
	curr = *sorted;
	prev = NULL;
	while (curr && curr->key)
	{
		n = max_strlen(new_node->key, curr->key);
		if (ft_strncmp(new_node->key, curr->key, n) <= 0)
			break ;
		prev = curr;
		curr = curr->next;
	}
	new_node->next = curr;
	if (!prev)
		*sorted = new_node;
	else
		prev->next = new_node;
}

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
