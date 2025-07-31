/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:24:01 by sabsanto          #+#    #+#             */
/*   Updated: 2025/07/31 01:34:44 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_raw_content(char *input, int start, int len, t_garbage **gc)
{
	char	*content;
	int		i;
	int		j;

	content = gc_malloc(len + 1, gc);
	if (!content)
		return (NULL);
	i = start;
	j = 0;
	while (j < len)
	{
		if (input[i] == '\\' && input[i + 1] == '"')
		{
			content[j++] = '"';
			i += 2;
		}
		else
			content[j++] = input[i++];
	}
	content[j] = '\0';
	return (content);
}

static char	*create_empty_string(t_garbage **gc)
{
	char	*str;

	str = gc_malloc(1, gc);
	if (str)
		str[0] = '\0';
	return (str);
}

char	*get_var_value(char *var_name, t_minishell *mini)
{
	char	*value;
	char	*exit_str;

	if (!var_name)
		return (create_empty_string(&mini->gc));
	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		exit_str = ft_itoa(mini->last_exit);
		if (!exit_str)
			return (create_empty_string(&mini->gc));
		value = gc_malloc(ft_strlen(exit_str) + 1, &mini->gc);
		if (!value)
		{
			free(exit_str);
			return (create_empty_string(&mini->gc));
		}
		ft_strlcpy(value, exit_str, ft_strlen(exit_str) + 1);
		free(exit_str);
		return (value);
	}
	value = get_env_value(mini->env, var_name);
	if (!value)
		return (create_empty_string(&mini->gc));
	return (value);
}

char	*extract_var_name(char *str, int *pos, t_garbage **gc)
{
	int		start;
	int		len;
	char	*var_name;

	start = *pos;
	if (str[*pos] == '?')
	{
		(*pos)++;
		var_name = gc_malloc(2, gc);
		if (!var_name)
			return (NULL);
		ft_strlcpy(var_name, "?", 2);
		return (var_name);
	}
	if (!(ft_isalpha(str[*pos]) || str[*pos] == '_'))
		return (NULL);
	while (str[*pos] && (ft_isalnum(str[*pos]) || str[*pos] == '_'))
		(*pos)++;
	len = *pos - start;
	var_name = gc_malloc(len + 1, gc);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, &str[start], len + 1);
	return (var_name);
}

char	*expand_variables(char *str, t_minishell *mini)
{
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	result = gc_malloc(1, &mini->gc);
	if (!result)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			result = process_dollar_sign(str, &i, result, mini);
		else
			result = append_char(result, str[i++], &mini->gc);
	}
	return (result);
}
