/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:36:33 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 11:01:06 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	handle_input_redirection(t_redir *redir, t_minishell *mini)
{
	int		fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: failed to open file");
		mini->last_exit = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: failed to duplicate");
		close(fd);
		mini->last_exit = 1;
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_input_redirections(t_commands *cmd, t_minishell *mini)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (handle_input_redirection(redir, mini) == -1)
				return (-1);
		}
		else if (redir->type == REDIR_HEREDOC)
		{
			if (handle_heredoc_redirection(redir, mini) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
