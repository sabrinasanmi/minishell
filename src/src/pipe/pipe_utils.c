/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:15:01 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/08 10:48:50 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Conta o número de comandos na lista
int	count_commands(t_commands *cmd_list)
{
	int			count;
	t_commands	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	setup_initial_vars(t_exec_data *data, t_commands *cmd_list)
{
	data->num_cmds = count_commands(cmd_list);
	data->i = 0;
	data->prev_read_fd = -1;
	data->cmd = cmd_list;
}

int	create_pipe_if_needed(t_commands *cmd, int pipe_fd[2])
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	return (0);
}
