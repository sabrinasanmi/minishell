/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:04:28 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/08 14:06:52 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_word_part(char *input, int *i, int len, t_minishell *mini)
{
	char	*part;
	int		start;
	int		word_len;

	if (input[*i] == '\'')
		part = handle_single_quotes(input, i, mini);
	else if (input[*i] == '"')
		part = handle_double_quotes(input, i, mini);
	else
	{
		start = *i;
		while (*i < len && !is_space(input[*i]) && !is_operator(input[*i])
			&& input[*i] != '\'' && input[*i] != '"')
		{
			(*i)++;
		}
		word_len = *i - start;
		part = gc_malloc(word_len + 1, &mini->gc_temp);
		if (!part)
			return (NULL);
		ft_strlcpy(part, &input[start], word_len + 1);
	}
	return (part);
}

static char	*collect_word_token(char *input, int *i, int len, t_minishell *mini)
{
	char	*result;
	char	*part;
	char	*joined;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	gc_add_ptr(result, &mini->gc_temp);
	while (*i < len && !is_space(input[*i]) && !is_operator(input[*i]))
	{
		part = extract_word_part(input, i, len, mini);
		if (!part)
			return (NULL);
		joined = ft_strjoin(result, part);
		if (!joined)
			return (NULL);
		gc_add_ptr(joined, &mini->gc_temp);
		result = joined;
	}
	return (result);
}

static void	process_word_token(char *input, int *i,
	t_token **tokens, t_minishell *mini)
{
	char	*value;
	t_token	*new_token;
	int		len;

	len = ft_strlen(input);
	value = collect_word_token(input, i, len, mini);
	if (value)
	{
		new_token = create_token(value, T_WORD, &mini->gc_temp);
		if (new_token)
			add_token_to_list(tokens, new_token);
	}
}

t_token	*tokenize(char *input, t_minishell *mini)
{
	t_token	*tokens;
	int		i;
	int		len;

	i = 0;
	tokens = NULL;
	if (!input || !mini)
		return (NULL);
	len = ft_strlen(input);
	while (i < len)
	{
		if (is_space(input[i]))
			i++;
		else if (is_operator(input[i]))
			process_operator_token(input, &i, &tokens, &mini->gc_temp);
		else
			process_word_token(input, &i, &tokens, mini);
	}
	return (tokens);
}
