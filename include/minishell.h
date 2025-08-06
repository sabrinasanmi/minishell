#ifndef MINISHELL_H
# define MINISHELL_H

/* Includes de sistema */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <stdarg.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>

# include <readline/readline.h>
# include <readline/history.h>

/* Includes locais */
# include "libft.h"
# include "garbage_collector.h"

/* Variável global para sinais (obrigatória por subject) */
extern volatile sig_atomic_t g_signal_received;

/* Variável global para ambiente */
extern char **__environ;

// struct de redirecionamentos
typedef enum e_redir_type
{
	REDIR_IN,      // <
	REDIR_OUT,     // >
	REDIR_APPEND,  // >>
	REDIR_HEREDOC  // <<
}	t_redir_type;

// possíveis tipos de tokens
typedef enum e_tokens
{
	T_WORD,         // palavra normal (comando ou argumento)
	T_PIPE,         // |
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_REDIR_HEREDOC
}	t_tokens;

typedef struct s_token
{
	t_tokens		type;     // T_WORD, T_PIPE, etc.
	char			*value;   // "cat", "|", "arquivo.txt", etc.
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_redir_type     type;   // tipo de redirecionamento
	char            *file;   // arquivo de entrada/saída ou delimitador (heredoc)
	struct s_redir  *next;   // lista encadeada de redirecionamentos
}	t_redir;

typedef struct s_commands
{
	char            **argv;      // vetor de strings: nome do comando e argumentos
	t_redir         *redir;     // lista de redirecionamentos
	struct s_commands *next;     // próximo comando (para pipeline)
}	t_commands;

typedef struct s_env
{
	char           *key;     // nome da variável (ex: PATH)
	char           *value;   // valor da variável (ex: "/usr/bin:/bin")
	struct s_env   *next;    // lista encadeada
}	t_env;

typedef struct s_minishell
{
	t_commands	*commands;	// lista de comandos (um por pipe)
	t_env		*env;		// lista de variáveis de ambiente
	int			last_exit;	//código de saída do último comando executado
	int			in_fd;		// STDIN original (para restaurar)
	int			out_fd;		// STDOUT original (para restaurar)
	t_garbage	*gc;
}	t_minishell;

typedef struct s_exec_data
{
	int			num_cmds;
	int			i;
	int			pipe_fd[2];
	int			prev_read_fd;
	t_commands	*cmd;
	pid_t		pid;
}	t_exec_data;

/* ============================================================================ */
/*                              PROTÓTIPOS PRINCIPAIS                          */
/* ============================================================================ */

/* Sinais */
void	signal_handler_interactive(int sig);
void	signal_handler_child(int sig);
void	setup_signals_interactive(void);
void	setup_signals_child(void);
void	setup_signals_ignore(void);

/* Parser Principal */
t_token		*tokenize(char *input, t_minishell *mini);
t_commands	*parse_tokens_to_commands(t_token *tokens, t_garbage **gc);

/* Parser Utils */
char    *extract_quoted_token(char *input, int *i, t_minishell *mini);
char    *extract_raw_content(char *input, int start, int len, t_garbage **gc);

/* Variable Expansion Utils */
char    *process_dollar_sign(char *str, int *i, char *result, t_minishell *mini);


/* Quotes e Expansão */
char	*handle_single_quotes(char *input, int *i, t_minishell *mini);
char	*handle_double_quotes(char *input, int *i, t_minishell *mini);
char	*expand_variables(char *str, t_minishell *mini);
char	*expand_exit_status(t_minishell *mini);
char	*extract_var_name(char *str, int *pos, t_garbage **gc);
char	*get_var_value(char *var_name, t_minishell *mini);

/* Command Builder */
t_commands	*create_command_node(t_garbage **gc);
t_redir		*create_redir_node(t_redir_type type, char *file, t_garbage **gc);
void		add_redir_to_command(t_commands *cmd, t_redir *new_redir);
int			add_arg_to_command(t_commands *cmd, char *arg, t_garbage **gc);
void		add_command_to_list(t_commands **list, t_commands *new_cmd);

/* Validações */
bool	is_valid_exit_arg(char *str);
bool	is_valid_identifier(const char *key);
bool	is_valid_id_export(const char *key);
bool	is_numeric_arg(char *str);

/* Utilitários */
char	is_space(char c);
char	is_operator(char c);
int		ft_atol(const char *nbr);
char	*join_strings(char *s1, char *s2, t_garbage **gc);
char	*append_char(char *str, char c, t_garbage **gc);

/* Ambiente */
void	init_env_list(t_minishell *mini, char **envp);
t_env	*find_env_key(t_env *env, const char *key);
t_env	*create_env_node(const char *key, const char *value, t_garbage **gc);
void	set_env_value(t_env **env, const char *key, const char *value, t_garbage **gc);
char	*get_env_value(t_env *env, const char *key);
char	**env_list_to_array(t_env *env, t_garbage **gc);

/* Builtins */
int		ft_echo(char **argv, t_minishell *mini);
int		ft_cd(char **argv, t_minishell *mini);
int		ft_pwd(t_minishell *mini);
int		ft_export(char **argv, t_minishell *mini);
int		ft_unset(char **argv, t_minishell *mini);
int		ft_env(t_minishell *mini);
int		ft_exit(char **argv, t_minishell *mini);

char	*ft_strjoin_and_free(char *s1, char *s2, int free_s);
size_t	max_strlen(const char *s1, const char *s2);
void	update_env_value(t_env *found, const char *value, t_garbage **gc);
void	append_env_node(t_env **env, t_env *new);
void	exec_input(char *input, t_minishell *mini);

/* Funções auxiliares do export */
t_env       *copy_env_node(t_env *src, t_garbage **gc);
void        insert_sorted_env_node(t_env **list, t_env *new_node);
void        print_env_line(t_env *node);
void        print_export_error(const char *arg);

/* Execução */
int		execute_pipeline(t_commands *cmd_list, t_minishell *mini);
int		exec_cmd(char **args, t_env *env, t_garbage **gc);
int		exec_single_command(t_exec_data *data, t_minishell *mini);
void	setup_initial_vars(t_exec_data *data, t_commands *cmd_list);
char	*get_cmd_path(char *cmd, t_env *env, t_garbage **gc);

/* Redirecionamentos */
int		handle_redirections(t_redir *redir_list, t_minishell *mini);
int		handle_input_redirection(t_redir *redir, t_minishell *mini);
int		setup_output_redirection(t_redir *redir);
int		handle_heredoc_redirection(t_redir *redir, t_minishell *mini);

/* Heredoc */
int		handle_heredoc(const char *delimiter, t_minishell *mini);
char	*read_line_stdin(t_garbage **gc);

/* Debug e Utilitários */
void	print_command_structure(t_commands *cmd_list);
void	free_array(char **arr);



#endif // MINISHELL_H
