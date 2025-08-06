/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:28:28 by makamins          #+#    #+#             */
/*   Updated: 2025/07/14 15:28:10 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	ft_env(t_minishell *mini)
{
	t_env	*curr_env;

	curr_env = mini->env;
	while (curr_env)
	{
		if (curr_env->value != NULL)
			printf("%s=%s\n", curr_env->key, curr_env->value);
		curr_env = curr_env->next;
	}
	return (0);
}
