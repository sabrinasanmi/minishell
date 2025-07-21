/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:00:55 by makamins          #+#    #+#             */
/*   Updated: 2025/07/14 15:29:15 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

bool	is_valid_identifier(const char *key)
{
	int	i;

	i = 1;
	if (!key || key[0] == '\0')
		return (false);
	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (false);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

void	remove_env_node(t_env **env, const char *key)
{
	t_env	*curr;
	t_env	*prev;
	
	if (!env || !*env)
		return ;
	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->key, key, ft_strlen(key) + 1) == 0)
		{
			if (!prev)
				*env = curr->next;
			else
				prev->next = curr->next;
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_unset(char **argv, t_minishell *mini)
{
	int	i;
	int	status;
	
	if (!argv || !mini)
		return (1);
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			write(2, "unset: `", 8);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "': not a valid identifier\n", 27);
			status = 1;
		}
		else
			remove_env_node(&mini->env, argv[i]);
		i++;
	}
	return (status);
}
