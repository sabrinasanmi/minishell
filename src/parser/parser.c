/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:04:28 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/04 14:52:41 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// Cria um novo token
static t_token	*create_token(char *value, t_tokens type, t_garbage **gc)
{
	t_token	*token;

	token = gc_malloc(sizeof(t_token), gc);
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

// Adiciona token à lista
static void	add_token_to_list(t_token **list, t_token *new_token)
{
	t_token	*last;

	if (!list || !new_token)
		return ;
	if (!*list)
	{
		*list = new_token;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new_token;
}

// Determina o tipo do operador
static t_tokens	get_operator_type(char *input, int pos)
{
	if (input[pos] == '|')
		return (T_PIPE);
	else if (input[pos] == '<')
	{
		if (input[pos + 1] == '<')
			return (T_REDIR_HEREDOC);
		return (T_REDIR_IN);
	}
	else if (input[pos] == '>')
	{
		if (input[pos + 1] == '>')
			return (T_REDIR_APPEND);
		return (T_REDIR_OUT);
	}
	return (T_WORD);
}

static void	process_operator_token(char *input, int *i, t_tokens **tokens, t_garbage **gc)
{
	t_tokens	type;
	t_tokens		*new_token;

	type = get_operator_type(input, *i);
	if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i])
	{
		new_token = create_token(NULL, type, gc);
		printf("Token: %c%c\n", input[*i], input[*i + 1]);  // Mantém o debug
		*i += 2;
	}
	else
	{
		new_token = create_token(NULL, type, gc);
		printf("Token: %c\n", input[*i]);  // Mantém o debug
		(*i)++;
	}
	if (new_token)
		add_token_to_list(tokens, new_token);
}

static void	process_quoted_token(char *input, int *i, t_minishell *mini, t_token **tokens)
{
	char	*value;
	t_token	*new_token;

	if (input[*i] == '\'')
		value = handle_single_quotes(input, i, mini);
	else if (input[*i] == '"')
		value = handle_double_quotes(input, i, mini);
	else
		value = NULL;
	if (!value)
		return ;
	printf("Token: %s\n", value);  // Mantém o debug
	new_token = create_token(value, T_WORD, &mini->gc);
	if (new_token)
		add_token_to_list(tokens, new_token);
}

static void	process_word_token(char *input, int *i, int len, t_minishell *mini, t_token **tokens)
{
	int		start;
	int		tok_len;
	char	*value;
	char	*expanded;
	t_token	*new_token;

	start = *i;
	while (*i < len && !is_space(input[*i]) && !is_operator(input[*i])
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	tok_len = *i - start;
	value = gc_malloc(tok_len + 1, &mini->gc);
	if (!value)
		return ;
	ft_strlcpy(value, &input[start], tok_len + 1);
	expanded = expand_variables(value, mini);
	if (expanded)
	{
		printf("Token: %s\n", expanded);  // Mantém o debug
		new_token = create_token(expanded, T_WORD, &mini->gc);
	}
	else
	{
		printf("Token: %s\n", value);  // Mantém o debug
		new_token = create_token(value, T_WORD, &mini->gc);
	}
	if (new_token)
		add_token_to_list(tokens, new_token);
}

t_token	*tokenize(char *input, t_minishell *mini)
{
	t_token	*tokens;
	int		i;
	int		len;

	if (!input)
		return (NULL);
	// Se mini não foi passado, cria um temporário
	t_minishell temp_mini = {0};
	if (!mini)
	{
		mini = &temp_mini;
		init_env_list(mini, __environ);
	}
	tokens = NULL;
	i = 0;
	len = ft_strlen(input);
	printf("Tokens encontrados:\n");

	while (i < len)
	{
		if (is_space(input[i]))
			i++;
		else if (is_operator(input[i]))
			process_operator_token(input, &i, &tokens, &mini->gc);
		else if (input[i] == '\'' || input[i] == '"')
			process_quoted_token(input, &i, mini, &tokens);
		else
			process_word_token(input, &i, len, mini, &tokens);
	}

	// Se usamos mini temporário, limpa
	if (mini == &temp_mini)
		gc_free_all(&mini->gc);

	return (tokens);
}

// Função de compatibilidade - mantém a interface antiga para debug
/*void	tokenize_debug(char *input)
{
	t_minishell	mini = {0};
	t_token		*tokens;

	init_env_list(&mini, __environ);
	tokens = tokenize(input, &mini);

	gc_free_all(&mini.gc);
}*/
void	tokenize_debug(void)
{
	t_minishell	mini = {0};

	init_env_list(&mini, __environ);
	gc_free_all(&mini.gc);
}

