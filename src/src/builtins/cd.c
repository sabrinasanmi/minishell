/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:39:05 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 18:58:23 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

static char	*get_target_path(char **argv, t_minishell *mini)
{
	char	*home;

	if (argv[1])
		return (argv[1]);
	home = get_env_value(mini->env, "HOME");
	if (!home)
	{
		write(2, "cd: HOME not set\n", 18);
		return (NULL);
	}
	return (home);
}

static char	*change_dir(const char *path)
{
	char	*cwd;

	if (chdir(path) != 0)
	{
		write(2, "cd: ", 4);
		perror(path);
		return (NULL);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("cd");
		return (NULL);
	}
	return (cwd);
}

static void	update_pwd_vars(t_minishell *mini,
	const char *oldpwd, const char *newpwd)
{
	set_env_value(&mini->env, "OLDPWD", oldpwd, &mini->gc_persistent);
	set_env_value(&mini->env, "PWD", newpwd, &mini->gc_persistent);
}

static int	prepare_cd_operation(char **argv, t_minishell *mini, char **oldpwd)
{
	int	argc;

	argc = 0;
	while (argv[argc])
		argc++;
	if (argc > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (-1);
	}
	*oldpwd = getcwd(NULL, 0);
	if (!*oldpwd)
	{
		perror("cd");
		return (-1);
	}
	gc_add_ptr(*oldpwd, &mini->gc_temp);
	return (0);
}

int	ft_cd(char **argv, t_minishell *mini)
{
	char	*oldpwd;
	char	*target;
	char	*newpwd;

	if (prepare_cd_operation(argv, mini, &oldpwd) == -1)
		return (1);
	target = get_target_path(argv, mini);
	if (!target)
		return (1);
	newpwd = change_dir(target);
	if (!newpwd)
		return (1);
	gc_add_ptr(newpwd, &mini->gc_temp);
	update_pwd_vars(mini, oldpwd, newpwd);
	return (0);
}
