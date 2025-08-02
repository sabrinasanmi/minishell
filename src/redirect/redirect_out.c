/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_out.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:21:05 by makamins          #+#    #+#             */
/*   Updated: 2025/08/01 15:15:06 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	setup_output_redirection(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_OUT)
		fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		return (-1);
	if (fd < 0)
	{
		perror(redir->file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_redirections(t_redir *redir_list)
{
	while (redir_list)
	{
		if (redir_list->type == REDIR_OUT || redir_list->type == REDIR_APPEND)
		{
			if (setup_output_redirection(redir_list) == -1)
				return (-1);
		}
		// (aqui vai IN e HEREDOC depois)
		redir_list = redir_list->next;
	}
	return (0);
}
