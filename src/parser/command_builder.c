/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:55:13 by sabsanto          #+#    #+#             */
/*   Updated: 2025/07/31 16:57:21 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cria um novo nó de comando
t_commands	*create_command_node(t_garbage **gc)
{
	t_commands	*cmd;

	cmd = gc_malloc(sizeof(t_commands), gc);
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	return (cmd);
}

// Cria um novo nó de redirecionamento
t_redir	*create_redir_node(t_redir_type type, char *file, t_garbage **gc)
{
	t_redir	*redir;

	redir = gc_malloc(sizeof(t_redir), gc);
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	redir->next = NULL;
	return (redir);
}

// Adiciona um redirecionamento ao comando
void	add_redir_to_command(t_commands *cmd, t_redir *new_redir)
{
	t_redir	*last;

	if (!cmd || !new_redir)
		return ;
	if (!cmd->redir)
	{
		cmd->redir = new_redir;
		return ;
	}
	last = cmd->redir;
	while (last->next)
		last = last->next;
	last->next = new_redir;
}

// Adiciona um argumento ao vetor argv do comando
int	add_arg_to_command(t_commands *cmd, char *arg, t_garbage **gc)
{
	char	**new_argv;
	int		count;
	int		i;

	if (!cmd || !arg)
		return (1);
	count = 0;
	while (cmd->argv && cmd->argv[count])
		count++;
	new_argv = gc_malloc(sizeof(char *) * (count + 2), gc);
	if (!new_argv)
		return (1);
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = arg;
	new_argv[i + 1] = NULL;
	cmd->argv = new_argv;
	return (0);
}

// Adiciona um comando ao final da lista de comandos
void	add_command_to_list(t_commands **list, t_commands *new_cmd)
{
	t_commands	*last;

	if (!list || !new_cmd)
		return ;
	if (!*list)
	{
		*list = new_cmd;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new_cmd;
}
