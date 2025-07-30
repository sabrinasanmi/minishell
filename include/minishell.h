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

# include <readline/readline.h>
# include <readline/history.h>

/*Includes de sistema que vamos usar em breve
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>*/

/* Includes locais */
# include "libft.h"
# include "garbage_collector.h"

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

/* Protótipos das funções de parsing */
void	tokenize(char *input);
char	*handle_single_quotes(char *input, int *i, t_minishell *mini);
char	*handle_double_quotes(char *input, int *i, t_minishell *mini);
char	*extract_quoted_token(char *input, int *i, t_minishell *mini);

/* Protótipos das funções de expansão */
char	*expand_variables(char *str, t_minishell *mini);
char	*extract_var_name(char *str, int *pos, t_garbage **gc);
char	*get_var_value(char *var_name, t_minishell *mini);
char	*extract_raw_content(char *input, int start, int len, t_garbage **gc);

/* Protótipos das funções auxiliares de string */
char	*join_strings(char *s1, char *s2, t_garbage **gc);
char	*append_char(char *str, char c, t_garbage **gc);

/* Protótipos existentes */
char 	is_space(char c);
char	is_operator(char c);
bool	is_numeric_arg(char *str);
int		ft_atol(const char *nbr);
char	*ft_strjoin_and_free(char *s1, char *s2, int free_s);

/* Funções de ambiente */
void		init_env_list(t_minishell *mini, char **envp);
void		update_env_value(t_env *found,
const char	*value, t_garbage **gc);
void		append_env_node(t_env **env, t_env *new);
void		set_env_value(t_env **env,
const char	*key, const char *value, t_garbage **gc);
t_env		*find_env_key(t_env *env, const char *key);
t_env		*create_env_node(const char *key,
const char	*value, t_garbage **gc);
char		*get_env_value(t_env *env, const char *key);

/* Builtins */
int		ft_cd(char **argv, t_minishell *mini);
int		ft_echo(char **argv, t_minishell *mini);
int		ft_env(t_minishell *mini);
int		ft_exit(char **argv, t_minishell *mini);
bool	is_valid_id_export(const char *key);
void	print_sorted_env(t_env *env, t_minishell *mini);
int		process_key(char *arg, t_minishell *mini);
int		export_variable(char *arg, t_minishell *mini);
int		ft_export(char **argv, t_minishell *mini);
int		ft_pwd(t_minishell *mini);
bool	is_valid_identifier(const char *key);
void	remove_env_node(t_env **env, const char *key);
int		ft_unset(char **argv, t_minishell *mini);
t_env	*copy_env_node(t_env *src, t_garbage **gc);
void	insert_sorted_env_node(t_env **sorted, t_env *new_node);
void	print_export_error(char *arg);

char **env_list_to_array(t_env *env, t_garbage **gc);
char	*get_cmd_path(char *cmd, t_env *env, t_garbage **gc);

int		exec_cmd(char **args, t_env *env, t_garbage **gc);
void	init_env_list(t_minishell *mini, char **envp);
void	exec_input(char *input, t_minishell *mini);
void	free_array(char **arr);

#endif // MINISHELL_H