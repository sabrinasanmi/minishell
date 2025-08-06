/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:30:58 by makamins          #+#    #+#             */
/*   Updated: 2025/08/04 15:16:49 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	wait_all_children(int n)
{
	int	status;
	int	i;
	
	i = 0;
	while (i < n)
	{
		wait(&status);
		i++;
	}
}

int	execute_pipeline(t_commands *cmd_list, t_minishell *mini)
{
	t_exec_data	data;
	int			last_status;
	
	// Inicializa as variáveis
	setup_initial_vars(&data, cmd_list);
	
	// Executa cada comando do pipeline
	while (data.cmd)
	{
		if (exec_single_command(&data, mini) == 1)
			return (1);
	}
	
	// Espera todos os processos filhos terminarem
	last_status = 0;
	while (data.num_cmds > 0)
	{
		int status;
		pid_t pid = wait(&status);
		if (pid == -1)
			break;
		// O status do último comando é o que importa
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		data.num_cmds--;
	}
	
	// Fecha o último fd se ainda estiver aberto
	if (data.prev_read_fd != -1)
		close(data.prev_read_fd);
	
	mini->last_exit = last_status;
	return (0);
}
