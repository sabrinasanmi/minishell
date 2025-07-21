/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:24:57 by makamins          #+#    #+#             */
/*   Updated: 2025/07/14 15:54:23 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	ft_exit(char **argv, t_minishell *mini)
{
	long	exit_code;
	bool	numeric;

	write(2, "exit\n", 5);
	if (!argv[1])
		exit(mini->last_exit);
	numeric = is_numeric_arg(argv[1]);
	if (!numeric)
	{
		write(2, "exit: numeric argument required\n", 32);
		exit(255);
	}
	if (argv[2])
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	exit_code = ft_atol(argv[1]);
	exit_code = exit_code % 256;
	exit(exit_code);
}
