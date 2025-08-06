/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:33:21 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/06 19:03:07 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_dollar_sign(char *str, int *i, char *result, t_minishell *mini)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
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
