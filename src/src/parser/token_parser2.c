/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:03:26 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 14:03:59 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	add_arg_to_command(t_commands *cmd, char *arg, t_garbage **gc)
{
	int		count;
	char	**new_argv;
	int		i;

	count = 0;
	if (cmd->argv)
	{
		while (cmd->argv[count])
		count++;
	}
	new_argv = gc_malloc(sizeof(char *) * (count + 2), gc);
	if (!new_argv)
		return (1);
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = arg;
	new_argv[i + 1] = NULL;
	cmd->argv = new_argv;
	return (0);
}
