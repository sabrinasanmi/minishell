/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:49:25 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/06 20:23:37 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

// Verifica se o comando é um builtin
static int	is_builtin(char *cmd)
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

// Executa um builtin
static int	execute_builtin(t_commands *cmd, t_minishell *mini)
{
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		return (ft_echo(cmd->argv, mini));
	else if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		return (ft_cd(cmd->argv, mini));
	else if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		return (ft_pwd(mini));
	else if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		return (ft_export(cmd->argv, mini));
	else if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		return (ft_unset(cmd->argv, mini));
	else if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		return (ft_env(mini));
	else if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		return (ft_exit(cmd->argv, mini));
	return (0);
}

// Executa comandos simples (sem pipes)
static void	execute_simple_command(t_commands *cmd, t_minishell *mini)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*cmd_path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;

	// Se é um builtin, executa diretamente
	if (is_builtin(cmd->argv[0]))
	{
		// Lidar com redirecionamentos para builtins
		int saved_stdin = -1;
		int saved_stdout = -1;
		
		// Salva file descriptors originais se houver redirecionamentos
		if (cmd->redir)
		{
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
		}
		
		// Aplica redirecionamentos
		if (cmd->redir && handle_redirections(cmd->redir, mini) == -1)
		{
			mini->last_exit = 1;
			return ;
		}
		
		// Executa o builtin
		mini->last_exit = execute_builtin(cmd, mini);
		
		// Restaura file descriptors originais
		if (saved_stdin != -1)
		{
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
		}
		if (saved_stdout != -1)
		{
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdout);
		}
	}
	else
	{
		// Comando externo - fork necessário
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			mini->last_exit = 1;
			return ;
		}
		else if (pid == 0)
		{
			// Processo filho
			// Aplica redirecionamentos
			if (cmd->redir && handle_redirections(cmd->redir, mini) == -1)
				exit(1);
			
			// Prepara o ambiente e executa
			envp = env_list_to_array(mini->env, &mini->gc_temp);
			if (!envp)
				exit(1);
			
			// Tenta encontrar e executar o comando
			cmd_path = get_cmd_path(cmd->argv[0], mini->env, &mini->gc_temp);
			if (!cmd_path)
			{
				write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
				write(2, ": command not found\n", 20);
				exit(127);
			}
			
			execve(cmd_path, cmd->argv, envp);
			perror("execve");
			exit(126);
		}
		else
		{
			// Processo pai espera
			if (waitpid(pid, &status, 0) == -1)
			{
				perror("waitpid");
				mini->last_exit = 1;
			}
			else if (WIFEXITED(status))
			{
				mini->last_exit = WEXITSTATUS(status);
			}
		}
	}
}

// Processa a linha de comando
static void	process_command_line(char *input, t_minishell *mini)
{
	t_token		*tokens;
	t_commands	*commands;

	// Tokeniza a entrada
	tokens = tokenize(input, mini);
	if (!tokens)
		return ;
	
	// Parseia tokens em comandos
	commands = parse_tokens_to_commands(tokens, &mini->gc_temp);
	if (!commands)
		return ;
	
	// Debug: imprime a estrutura de comandos (remover em produção)
	// print_command_structure(commands);
	
	// Executa os comandos
	if (commands && !commands->next)
	{
		// Comando único (sem pipes)
		execute_simple_command(commands, mini);
	}
	else if (commands && commands->next)
	{
		// Pipeline de comandos
		execute_pipeline(commands, mini);
	}
}

// Limpeza do readline
static void cleanup_readline(void)
{
	clear_history();
	rl_clear_pending_input();
	rl_cleanup_after_signal();
}

int main(void)
{
    char *input;
    t_minishell mini;
    
    // Inicializa AMBOS os garbage collectors
    mini = (t_minishell){0};
    mini.last_exit = 0;
    mini.in_fd = STDIN_FILENO;
    mini.out_fd = STDOUT_FILENO;
    mini.gc_persistent = NULL;  // Para ambiente
    mini.gc_temp = NULL;        // Para comandos temporários
    
    // Ambiente usa gc_persistent
    init_env_list(&mini, __environ);
    setup_signals_interactive();
    
    while (1)
    {
        input = readline("minishell> ");
        
        if (!input)
        {
            printf("exit\n");
            break;
        }
        
        if (g_signal_received == SIGINT)
        {
            mini.last_exit = 130;
            g_signal_received = 0;
        }
        
        if (*input)
        {
            add_history(input);
            process_command_line(input, &mini);
        }
        
        free(input);
        
        // MUDANÇA CRÍTICA: libera apenas dados temporários
        gc_free_all(&mini.gc_temp);  // ← SÓ o temporário!
        // mini.gc_persistent permanece intacto
    }
    
    // Limpeza final: libera TUDO
    cleanup_readline();
    rl_clear_history();
    gc_free_all(&mini.gc_temp);
    gc_free_all(&mini.gc_persistent);
    
    return (mini.last_exit);
}

