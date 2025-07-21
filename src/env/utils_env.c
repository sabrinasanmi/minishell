/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:04:12 by makamins          #+#    #+#             */
/*   Updated: 2025/07/14 15:29:29 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

t_env	*find_env_key(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

t_env	*create_env_node(const char *key,
	const char *value, t_garbage **gc)
{
	t_env	*node;

	node = gc_malloc(sizeof(t_env), gc);
	if (!node)
		return (NULL);
	node->key = gc_malloc(ft_strlen(key) + 1, gc);
	if (!node->key)
		return (NULL);
	ft_strlcpy(node->key, key, ft_strlen(key) + 1);
	node->value = gc_malloc(ft_strlen(value) + 1, gc);
	if (!node->value)
		return (NULL);
	ft_strlcpy(node->value, value, ft_strlen(value) + 1);
	node->next = NULL;
	return (node);
}

void	update_env_value(t_env *found,
	const char *value, t_garbage **gc)
{
	char	*new_value;

	new_value = gc_malloc(ft_strlen(value) + 1, gc);
	if (!new_value)
		return ;
	ft_strlcpy(new_value, value, ft_strlen(value) + 1);
	found->value = new_value;
}

void	append_env_node(t_env **env, t_env *new)
{
	t_env	*last;

	if (!*env)
	{
		*env = new;
		return ;
	}
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new;
}

void	set_env_value(t_env **env,
	const char *key, const char *value, t_garbage **gc)
{
	t_env	*found;
	t_env	*new;

	if (!env || !key || !value || !gc)
		return ;
	found = find_env_key(*env, key);
	if (found)
		update_env_value(found, value, gc);
	else
	{
		new = create_env_node(key, value, gc);
		if (!new)
			return ;
		append_env_node(env, new);
	}
}

/*
** Funções utilitárias para manipulação da lista de variáveis de ambiente.
** - create_env_node: cria um novo nó com key e value usando gc_malloc.
** - find_env_key: busca uma key na lista e retorna o nó correspondente.
** - update_env_value: atualiza o value de uma key existente com novo conteúdo.
** - append_env_node: adiciona um novo nó no final da lista de variáveis.
** - set_env_value: atualiza ou cria uma variável de ambiente na lista.
*/
