/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:08:47 by makamins          #+#    #+#             */
/*   Updated: 2025/08/05 23:13:36 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

bool	is_valid_id_export(const char *key)
{
	int	i;

	if (!key || key[0] == '\0')
		return (false);
	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (false);
	i = 0;
	while (key[i] && key[i] != '=')
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

void	print_env_line(t_env *node)
{
	if (!node || !node->key)
		return ;
	printf("declare -x %s", node->key);
	if (node->value)
		printf("=\"%s\"", node->value);
	printf("\n");
}
