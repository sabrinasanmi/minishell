/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:33:21 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/08 10:40:19 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_dollar_sign(char *str, int *i, char *result, t_minishell *mini)
{
	char	*var_name;
	char	*var_value;
	char	*exit_str;

	(*i)++;
	if (str[*i] == '?')
	{
		exit_str = ft_itoa(mini->last_exit);
		if (!exit_str)
			return (NULL);
		gc_add_ptr(exit_str, &mini->gc_temp);
		(*i)++;
		return (join_strings(result, exit_str, &mini->gc_temp));
	}
	var_name = extract_var_name(str, i, &mini->gc_temp);
	if (var_name)
	{
		var_value = get_var_value(var_name, mini);
		result = join_strings(result, var_value, &mini->gc_temp);
	}
	else
		result = join_strings(result, "$", &mini->gc_temp);
	return (result);
}
