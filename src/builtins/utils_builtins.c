/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:56:34 by makamins          #+#    #+#             */
/*   Updated: 2025/07/14 16:05:00 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "minishell.h"

t_env	*copy_env_node(t_env *src, t_garbage **gc)
{
	t_env	*new_node;
	
	if (!src)
		return (NULL);
	new_node = gc_malloc(sizeof(t_env), gc);
	if (!new_node)
		return (NULL);
	new_node->key = gc_malloc(ft_strlen(src->key) + 1, gc);
	if (!new_node->key)
		return (NULL);
	ft_strlcpy(new_node->key, src->key, ft_strlen(src->key) + 1);
	if (src->value)
	{
		new_node->value = gc_malloc(ft_strlen(src->value) + 1, gc);
		if (!new_node->value)
			return (NULL);
		ft_strlcpy(new_node->value, src->value, ft_strlen(src->value) + 1);
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);	
}

void	insert_sorted_env_node(t_env **sorted, t_env *new_node)
{
	t_env	*curr;
	t_env	*prev;

	if (!sorted || !new_node)
		return ;
	curr = *sorted;
	prev = NULL;
	while (curr && ft_strncmp(curr->key, new_node->key, ft_strlen(curr->key)))
	{
		prev = curr;
		curr = curr->next;
	}
	if (!prev)
	{
		new_node->next = *sorted;
		*sorted = new_node;
	}
	else
	{
		new_node->next = curr;
		prev->next = new_node;
	}
}

void	print_export_error(char *arg)
{
	write(2, "export: `", 9);
	write(2, arg, ft_strlen(arg));
	write(2, "`: not a valid identifier\n", 27);
}
