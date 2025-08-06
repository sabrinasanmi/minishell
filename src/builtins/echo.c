/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:16:50 by makamins          #+#    #+#             */
/*   Updated: 2025/08/05 23:21:35 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	ft_echo(char **argv, t_minishell *mini)
{
	int		i;
	bool	newline;

	(void)mini;
	i = 1;
	newline = true;
	
	// Processar múltiplos flags -n consecutivos
	while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		int j = 1;
		while (argv[i][j] == 'n')
			j++;
		if (argv[i][j] == '\0')
		{
			newline = false;
			i++;
		}
		else
			break;
	}
	// Imprimir argumentos
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
