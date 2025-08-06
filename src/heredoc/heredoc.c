/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:47:21 by makamins          #+#    #+#             */
/*   Updated: 2025/08/05 23:11:47 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

char	*append_char_to_line(char *line, char c, int len, t_garbage **gc)
{
	char	*new_line;

	new_line = gc_malloc(len + 2, gc);
	if (!new_line)
		return (NULL);
	ft_strlcpy(new_line, line, len + 1);
	new_line[len] = c;
	new_line[len + 1] = '\0';
	return (new_line);
}

char	*read_line_stdin(t_garbage **gc)
{
	char	buff[1];
	char	*line;
	int		i;
	int		len;

	len = 0;
	line = gc_malloc(1, gc);
	if (!line)
		return (NULL);
	line[0] = '\0';
	while ((i = read(STDIN_FILENO, buff, 1)) > 0)
	{
		if (buff[0] == '\n')
			break ;
		line = append_char_to_line(line, buff[0], len, gc);
		if (!line)
			return (NULL);
		len++;
	}
	if (i == -1)
		return (NULL);
	return (line);
}

static int	heredoc_loop(int write_fd, const char *delimiter, t_minishell *mini)
{
	char		*line;

	while (1)
	{
		write(1, "> ", 2);
		line = read_line_stdin(&mini->gc);
		if (!line)
			return (-1);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			break ;
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
	}
	return (0);
}

int	handle_heredoc(const char *delimiter, t_minishell *mini)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (heredoc_loop(pipe_fd[1], delimiter, mini) == - 1)
	{
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	handle_heredoc_redirection(t_redir *redir, t_minishell *mini)
{
	int	fd;

	fd = handle_heredoc(redir->file, mini);
	if (fd == -1)
	{
		mini->last_exit = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		mini->last_exit = 1;
		return (-1);
	}
	close(fd);
	return (0);
}
