/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:12:14 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 20:33:50 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>

// garbage collector
typedef struct s_garbage
{
	void				*ptr;
	struct s_garbage	*next;
}	t_garbage;

void	*gc_malloc(size_t size, t_garbage **gc);
int		gc_add_ptr(void *ptr, t_garbage **gc);
void	gc_free_all(t_garbage **gc);
void	gc_free_temp_only(t_garbage **gc_temp, t_garbage **gc_persistent);

#endif