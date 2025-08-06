/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:35:36 by makamins          #+#    #+#             */
/*   Updated: 2025/08/06 16:06:51 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/garbage_collector.h"

void	*gc_malloc(size_t size, t_garbage **gc)
{
	void		*node;
	t_garbage	*gc_node;

	node = malloc(size);
	if (!node)
		return (NULL);
	gc_node = malloc(sizeof(t_garbage));
	if (!gc_node)
	{
		free(gc_node);
		return (NULL);
	}
	gc_node->ptr = node;
	gc_node->next = *gc;
	*gc = gc_node;
	return (node);
}

int	gc_add_ptr(void *ptr, t_garbage **gc)
{
	t_garbage	*new_node;

	if (!gc || !*gc)
	{
		fprintf(stderr, "ERRO: gc ou *gc é NULL ao tentar adicionar ponteiro %p\n", ptr);
		return (1);  // Erro!
	}

	new_node = malloc(sizeof(t_garbage));
	if (!new_node)
	{
		fprintf(stderr, "ERRO: malloc falhou em gc_add_ptr ao adicionar ponteiro %p\n", ptr);
		return (1);  // Erro!
	}

	new_node->ptr = ptr;
	new_node->next = *gc;
	*gc = new_node;

	printf("DEBUG: gc_add_ptr adicionou ptr = %p com sucesso\n", ptr);
	return (0);  // Sucesso!
}


void	gc_free_all(t_garbage **gc)
{
	t_garbage	*temp;

	while (*gc)
	{
		temp = *gc;
		free(temp->ptr);
		*gc = temp->next;
		free(temp);
	}
}
// int main(void)
// {
// 	t_garbage *gc = NULL; // Começa vazio

// 	// Aloca uma string com gc_malloc
// 	char *nome = gc_malloc(100, &gc);
// 	if (!nome)
// 	{
// 		printf("Erro ao alocar memória\n");
// 		return (1);
// 	}
// 	strcpy(nome, "Maíra Rainha do Minishell");
// 	printf("Nome alocado: %s\n", nome);

// 	// Aloca um vetor de inteiros também
// 	int *numeros = gc_malloc(sizeof(int) * 5, &gc);
// 	if (!numeros)
// 	{
// 		printf("Erro ao alocar números\n");
// 		gc_free_all(&gc);
// 		return (1);
// 	}
// 	for (int i = 0; i < 5; i++)
// 		numeros[i] = i * 10;

// 	printf("Números: ");
// 	for (int i = 0; i < 5; i++)
// 		printf("%d ", numeros[i]);
// 	printf("\n");

// 	// Agora libera tudo com uma única função
// 	gc_free_all(&gc);

// 	// Verifica se limpou
// 	if (gc == NULL)
// 		printf("Memória limpa com sucesso!\n");

// 	return (0);
// }