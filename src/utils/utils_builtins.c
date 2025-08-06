/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:56:34 by makamins          #+#    #+#             */
/*   Updated: 2025/08/06 00:08:48 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "minishell.h"

t_env	*copy_env_node(t_env *src, t_garbage **gc)
{
	t_env	*new_node;
	size_t	key_len;
	size_t	value_len;

	if (!src || !src->key)
		return (NULL);
	new_node = gc_malloc(sizeof(t_env), gc);
	if (!new_node)
		return (NULL);
	key_len = ft_strlen(src->key) + 1;
	new_node->key = gc_malloc(key_len, gc);
	if (!new_node->key)
		return (NULL);
	ft_strlcpy(new_node->key, src->key, key_len);
	if (src->value)
	{
		value_len = ft_strlen(src->value) + 1;
		new_node->value = gc_malloc(value_len, gc);
		if (!new_node->value)
			return (NULL);
		ft_strlcpy(new_node->value, src->value, value_len);
	}
	else
		new_node->value = NULL;
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
	while (curr)
	{
		if (!curr->key)
			break ;
		n = max_strlen(new_node->key, curr->key);
		if (ft_strncmp(new_node->key, curr->key, n) <= 0)
			break ;
		prev = curr;
		curr = curr->next;
	}
	if (!prev)
		*sorted = new_node;
	else
		prev->next = new_node;
	new_node->next = curr;
}

// void	insert_sorted_env_node(t_env **sorted, t_env *new_node)
// {
// 	t_env	*curr;
// 	t_env	*prev;

// 	if (!sorted || !new_node || !new_node->key)
// 		return ;
// 	curr = *sorted;
// 	prev = NULL;
// 	while (curr && curr->key &&
// 		ft_strncmp(new_node->key, curr->key, max_strlen(new_node->key, curr->key)) > 0)
// 	{
// 		prev = curr;
// 		curr = curr->next;
// 	}
// 	if (!prev)
// 	{
// 		new_node->next = *sorted;
// 		*sorted = new_node;
// 	}
// 	else
// 	{
// 		new_node->next = curr;
// 		prev->next = new_node;
// 	}
// }

void	print_export_error(const char *arg)
{
	write(2, "export: `", 9);
	write(2, arg, ft_strlen(arg));
	write(2, "`: not a valid identifier\n", 27);
}
