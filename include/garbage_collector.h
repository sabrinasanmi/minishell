#ifndef GC_H
# define GC_H

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

#endif