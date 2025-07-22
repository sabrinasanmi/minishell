/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:04:28 by sabsanto          #+#    #+#             */
/*   Updated: 2025/07/21 21:23:01 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_operator_token(char *input, int *i)
{
	if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i])
	{
		printf("Token: %c%c\n", input[*i], input[*i + 1]);
		*i += 2;
	}
	else
	{
		printf("Token: %c\n", input[*i]);
		(*i)++;
	}
}

static void	process_quoted_token(char *input, int *i, t_minishell *mini)
{
	char	*token;

	if (input[*i] == '\'')
		token = handle_single_quotes(input, i);
	else if (input[*i] == '"')
		token = handle_double_quotes(input, i, mini);
	else
		token = NULL;
	if (!token)
		return ;
	printf("Token: %s\n", token);
	free(token);
}

static void	process_word_token(char *input, int *i, int len)
{
	int		start;
	int		tok_len;
	char	*token;

	start = *i;
	while (*i < len && !is_space(input[*i]) && !is_operator(input[*i])
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	tok_len = *i - start;
	token = malloc(tok_len + 1);
	if (!token)
		return ;
	ft_strlcpy(token, &input[start], tok_len + 1);
	printf("Token: %s\n", token);
	free(token);
}

void	tokenize(char *input)
{
	t_minishell	mini;
	int			i;
	int			len;

	if (!input)
		return ;
	mini = (t_minishell){0};
	init_env_list(&mini, __environ);
	i = 0;
	len = ft_strlen(input);
	while (i < len)
	{
		if (is_space(input[i]))
			i++;
		else if (is_operator(input[i]))
			process_operator_token(input, &i);
		else if (input[i] == '\'' || input[i] == '"')
			process_quoted_token(input, &i, &mini);
		else
			process_word_token(input, &i, len);
	}
	gc_free_all(&mini.gc);
}
