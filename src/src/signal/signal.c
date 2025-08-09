/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:44:21 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/06 21:05:54 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

// NÃO DECLARAR g_signal_received aqui! Já está em minishell_main.c

void	signal_handler_interactive(int sig)
{
	g_signal_received = sig;
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_handler_child(int sig)
{
	g_signal_received = sig;
	if (sig == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (sig == SIGQUIT)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
}

void	setup_signals_interactive(void)
{
	signal(SIGINT, signal_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_child(void)
{
	signal(SIGINT, signal_handler_child);
	signal(SIGQUIT, signal_handler_child);
}

void	setup_signals_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
