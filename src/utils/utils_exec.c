/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:01:34 by makamins          #+#    #+#             */
/*   Updated: 2025/07/30 19:07:31 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	exec_input(char *input, t_minishell *mini)
{
	char	**args;

	args = ft_split(input, ' '); // isso aqui é temporário, até ter parser completo
	if (!args || !args[0])
	{
		if (args)
			free_array(args); // ← necessário se args foi alocado!
		return ;
	}
	if (strcmp(args[0], "env") == 0)
		ft_env(mini);
	else if (strcmp(args[0], "export") == 0)
		ft_export(args, mini);
	else if (strcmp(args[0], "unset") == 0)
		ft_unset(args, mini);
	else
		exec_cmd(args, mini->env, &mini->gc); // executa comando externo

	free_array(args);
}

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
