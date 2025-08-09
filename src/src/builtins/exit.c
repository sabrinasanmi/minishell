/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:24:57 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 19:00:19 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

bool	is_valid_exit_arg(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (false);
	if (str[0] == '-' || str[0] == '+')
		i++;
	if (str[i] == '\0')
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	ft_exit(char **argv, t_minishell *mini)
{
	long	exit_code;

	write(2, "exit\n", 5);
	if (!argv[1])
		exit(mini->last_exit);
	if (!is_valid_exit_arg(argv[1]))
	{
		write(2, "exit: numeric argument required\n", 32);
		exit(2);
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
