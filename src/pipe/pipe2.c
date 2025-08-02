/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:30:58 by makamins          #+#    #+#             */
/*   Updated: 2025/07/31 18:54:46 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	wait_all_children(int n)
{
	int	status;
	
	while (n-- > 0)
		wait(&status);
}

int	execute_pipeline(t_commands *cmd_list, t_minishell *mini)
{
	t_exec_data	data;
	
	setup_initial_vars(&data, cmd_list);
	while (data.cmd)
	{
		if (exec_single_command(&data, mini) == 1)
			return (1);
	}
	wait_all_children(data.num_cmds);
	return (0);
}
