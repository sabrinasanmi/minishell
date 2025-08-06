/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:46:32 by makamins          #+#    #+#             */
/*   Updated: 2025/08/06 19:55:29 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

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

// Verifica se o comando é um builtin
static int	is_builtin_cmd(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

// Executa builtin no processo filho
static void	execute_builtin_in_child(t_commands *cmd, t_minishell *mini)
{
	int	result;

	result = 0;
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		result = ft_echo(cmd->argv, mini);
	else if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		result = ft_cd(cmd->argv, mini);
	else if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		result = ft_pwd(mini);
	else if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		result = ft_export(cmd->argv, mini);
	else if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		result = ft_unset(cmd->argv, mini);
	else if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		result = ft_env(mini);
	else if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		result = ft_exit(cmd->argv, mini);
	exit(result);
}

void	child_procces_logic(t_commands *cmd, int prev_read_fd, 
	int pipe_fd[2], t_minishell *mini)
{
	char	**envp;
	char	*cmd_path;

	// Configura redirecionamento de entrada (do pipe anterior)
	if (prev_read_fd != -1)
	{
		dup2(prev_read_fd, STDIN_FILENO);
		close(prev_read_fd);
	}
	
	// Configura redirecionamento de saída (para o próximo pipe)
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (handle_redirections(cmd->redir, mini) == -1)
		exit(1);
	
	// Se for builtin, executa diretamente
	if (is_builtin_cmd(cmd->argv[0]))
	{
		execute_builtin_in_child(cmd, mini);
	}
	
	// Comando externo
	envp = env_list_to_array(mini->env, &mini->gc_temp);
	if (!envp)
		exit(1);
		
	cmd_path = get_cmd_path(cmd->argv[0], mini->env, &mini->gc_temp);
	if (!cmd_path)
	{
		printf("%s: command not found 3\n", cmd->argv[0]);
		exit(127);
	}
	
	execve(cmd_path, cmd->argv, envp);
	perror(cmd->argv[0]);
	exit(127);
}

void	parent_procces_logic(int *prev_read_fd,
	int pipe_fd[2], t_commands *cmd)
{
	// Fecha o fd do pipe anterior se existir
	if (*prev_read_fd != -1)
		close(*prev_read_fd);
		
	// Se houver próximo comando, salva o fd de leitura do pipe atual
	if (cmd->next)
	{
		close(pipe_fd[1]);  // Fecha a escrita
		*prev_read_fd = pipe_fd[0];  // Salva a leitura para o próximo comando
	}
}

int	exec_single_command(t_exec_data *data, t_minishell *mini)
{
	// Cria pipe se necessário
	if (create_pipe_if_needed(data->cmd, data->pipe_fd) == -1)
		return (1);
		
	data->pid = fork();
	if (data->pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (data->pid == 0)
	{
		// Processo filho
		child_procces_logic(data->cmd, data->prev_read_fd,
			data->pipe_fd, mini);
	}
	else
	{
		// Processo pai
		parent_procces_logic(&data->prev_read_fd,
			data->pipe_fd, data->cmd);
	}
	
	// Avança para o próximo comando
	data->cmd = data->cmd->next;
	data->i++;
	return (0);
}
