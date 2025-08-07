/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:49:25 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/06 20:52:14 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

// Variável global para sinais
volatile sig_atomic_t	g_signal_received = 0;

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
		
		if (cmd->redir)
		{
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
		}
		
		if (cmd->redir && handle_redirections(cmd->redir, mini) == -1)
		{
			mini->last_exit = 1;
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
			return ;
		}
		
		mini->last_exit = execute_builtin(cmd, mini);
		
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
			else
			{
				if (WIFEXITED(status))
					mini->last_exit = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					mini->last_exit = 128 + WTERMSIG(status);
			}
		}
	}
}

// Processa uma linha de comando
static void	process_command_line(char *input, t_minishell *mini)
{
	t_token		*tokens;
	t_commands	*cmd_list;

	// Tokeniza a entrada
	tokens = tokenize(input, mini);
	if (!tokens)
		return ;
	
	// Converte tokens em comandos
	cmd_list = parse_tokens_to_commands(tokens, &mini->gc_temp);
	if (!cmd_list)
		return ;
	
	// Salva lista de comandos
	mini->commands = cmd_list;
	
	// Executa comandos
	if (cmd_list->next)
	{
		// Pipeline
		mini->last_exit = execute_pipeline(cmd_list, mini);
	}
	else
	{
		// Comando simples
		execute_simple_command(cmd_list, mini);
	}
}

// Inicializa a estrutura minishell
static void	init_minishell(t_minishell *mini, char **envp)
{
	// Inicializa garbage collectors
	mini->gc_persistent = NULL;
	mini->gc_temp = NULL;
	
	// Inicializa estrutura
	mini->commands = NULL;
	mini->env = NULL;
	mini->last_exit = 0;
	mini->in_fd = dup(STDIN_FILENO);
	mini->out_fd = dup(STDOUT_FILENO);
	
	// Inicializa ambiente - IMPORTANTE: passar envp corretamente
	init_env_list(mini, envp);
	
	// Configura sinais
	setup_signals_interactive();
}

// Função principal
int	main(int argc, char **argv, char **envp)
{
	t_minishell	mini;
	char		*input;
	
	(void)argc;
	(void)argv;
	
	// Inicializa o minishell com o ambiente
	init_minishell(&mini, envp);
	
	// Loop principal
	while (1)
	{
		// Limpa garbage collector temporário
		gc_free_all(&mini.gc_temp);
		
		// Lê entrada do usuário
		input = readline("minishell> ");
		if (!input)
		{
			// EOF (Ctrl-D)
			write(1, "exit\n", 5);
			break ;
		}
		
		// Ignora linhas vazias
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		
		// Adiciona ao histórico
		add_history(input);
		
		// Processa o comando
		process_command_line(input, &mini);
		
		// Libera a entrada
		free(input);
		
		// Verifica sinais
		if (g_signal_received)
		{
			if (g_signal_received == SIGINT)
				mini.last_exit = 130;
			g_signal_received = 0;
		}
	}
	
	// Limpa tudo antes de sair
	gc_free_all(&mini.gc_persistent);
	gc_free_all(&mini.gc_temp);
	
	// Restaura file descriptors
	dup2(mini.in_fd, STDIN_FILENO);
	dup2(mini.out_fd, STDOUT_FILENO);
	close(mini.in_fd);
	close(mini.out_fd);
	
	return (mini.last_exit);
}

