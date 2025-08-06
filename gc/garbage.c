/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:35:36 by makamins          #+#    #+#             */
/*   Updated: 2025/08/06 19:09:54 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/garbage_collector.h"

void	*gc_malloc(size_t size, t_garbage **gc)
{
	void		*ptr;
	t_garbage	*gc_node;

	if (!gc)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	gc_node = malloc(sizeof(t_garbage));
	if (!gc_node)
	{
		free(ptr);
		return (NULL);
	}
	gc_node->ptr = ptr;
	gc_node->next = *gc;
	*gc = gc_node;
	return (ptr);
}

int	gc_add_ptr(void *ptr, t_garbage **gc)
{
	t_garbage	*new_node;

	if (!ptr || !gc)
		return (0);
	new_node = malloc(sizeof(t_garbage));
	if (!new_node)
		return (0);
	new_node->ptr = ptr;
	new_node->next = *gc;
	*gc = new_node;
	return (1);
}

void	gc_free_all(t_garbage **gc)
{
	t_garbage	*temp;

	if (!gc)
		return ;
	while (*gc)
	{
		temp = *gc;
		free(temp->ptr);
		*gc = temp->next;
		free(temp);
	}
}
void gc_free_temp_only(t_garbage **gc_temp, t_garbage **gc_persistent)
{
    t_garbage *temp;
    
    (void)gc_persistent;  // ← Adicionar esta linha
    
    if (!gc_temp)
        return;
    
    // Libera apenas o garbage collector temporário
    while (*gc_temp)
    {
        temp = *gc_temp;
        free(temp->ptr);
        *gc_temp = temp->next;
        free(temp);
    }
    // gc_persistent permanece intacto
}

