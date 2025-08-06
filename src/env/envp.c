/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:25:19 by makamins          #+#    #+#             */
/*   Updated: 2025/08/06 20:23:25 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

// Converte o vetor envp (KEY=VALUE) em uma lista 
//ligada de variáveis de ambiente internas
void init_env_list(t_minishell *mini, char **envp)
{
    int i;
    char *sep;
    char *key;
    char *value;
    
    if (!mini || !envp)
        return;
    
    i = 0;
    while (envp[i])
    {
        sep = ft_strchr(envp[i], '=');
        if (!sep)
        {
            // USA gc_persistent para ambiente
            set_env_value(&mini->env, envp[i], NULL, &mini->gc_persistent);
            i++;
            continue;
        }
        
        // IMPORTANTE: usar gc_persistent
        key = gc_malloc(sep - envp[i] + 1, &mini->gc_persistent);
        if (!key)
        {
            i++;
            continue;
        }
        
        ft_strlcpy(key, envp[i], sep - envp[i] + 1);
        value = sep + 1;
        set_env_value(&mini->env, envp[i], value, &mini->gc_persistent);
        i++;
    }
}


static int	count_env_entries(t_env *env)
{
	int		count;
	t_env	*curr;

	count = 0;
	curr = env;
	while (curr)
	{
		if (curr->key && curr->value)
			count++;
		curr = curr->next;
	}
	return (count);
}

// Converte a lista encadeada de variáveis de ambiente (t_env) em um array
// de strings no formato "KEY=VALUE", para uso em execve e similares.
static char	*make_env_entry(t_env *env, t_garbage **gc)
{
	size_t	total_len;
	char	*entry;

	if (!env || !env->key || !env->value || !gc)
		return (NULL);
		
	total_len = ft_strlen(env->key) + ft_strlen(env->value) + 2;
	entry = gc_malloc(total_len, gc);
	if (!entry)
		return (NULL);
		
	ft_strlcpy(entry, env->key, total_len);
	ft_strlcat(entry, "=", total_len);
	ft_strlcat(entry, env->value, total_len);
	return (entry);
}

char	**env_list_to_array(t_env *env, t_garbage **gc)
{
	char	**array;
	int		i;
	t_env	*curr;
	char	*entry;

	if (!env || !gc)
		return (NULL);
		
	array = gc_malloc(sizeof(char *) * (count_env_entries(env) + 1), gc);
	if (!array)
		return (NULL);
		
	i = 0;
	curr = env;
	while (curr)
	{
		if (curr->key && curr->value)
		{
			entry = make_env_entry(curr, gc);
			if (entry)
				array[i++] = entry;
		}
		curr = curr->next;
	}
	array[i] = NULL;
	return (array);
}
