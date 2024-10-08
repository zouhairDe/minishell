/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 17:15:02 by zouddach          #+#    #+#             */
/*   Updated: 2024/07/12 13:49:41 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_set_env(t_list *env, char *name, char *value)
{
	t_list	*head;

	head = env;
	name = ft_strjoin(name, "=");
	while (env)
	{
		if (!ft_strncmp(env->content, name, ft_strlen(name)))
		{
			free(env->content);
			env->content = ft_strjoin(name, value);
			if (!env->content)
				return (EXIT_FAILURE);
			free(name);
			return (EXIT_SUCCESS);
		}
		env = env->next;
	}
	if (!head)
	{
		name = ft_strjoin(name, value);
		if (!name)
			return (EXIT_FAILURE);
		ft_lstadd_back(&head, ft_lstnew(name));
	}
	return (EXIT_SUCCESS);
}

int	ft_first_condition(t_shell *shell)
{
	if (ft_getenv("HOME", shell->env) == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR);
		return (EXIT_FAILURE);
	}
	if (chdir(ft_getenv("HOME", shell->env)) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR);
		ft_putstr_fd(ft_getenv("HOME", shell->env), STDERR);
		ft_putstr_fd(": No such file or directory\n", STDERR);
		return (EXIT_FAILURE);
	}
	if (ft_set_env(shell->env, "OLDPWD", ft_getenv("PWD", shell->env)))
		return (EXIT_FAILURE);
	if (ft_set_env(shell->env, "PWD", ft_getenv("HOME", shell->env)))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_second_condition(t_shell *shell, char *pwd)
{
	if (chdir(ft_getenv("OLDPWD", shell->env)) != 0)
	{
		ft_putstr_fd("minishell : cd: ", STDERR);
		ft_putstr_fd(ft_getenv("OLDPWD", shell->env), STDERR);
		ft_putstr_fd(": No such file or directory\n", STDERR);
		return (EXIT_FAILURE);
	}
	if (ft_set_env(shell->env, "OLDPWD", ft_getenv("PWD", shell->env)))
		return (EXIT_FAILURE);
	if (ft_set_env(shell->env, "PWD", getcwd(pwd, 255)))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_cd_error(char *path, t_shell *shell)
{
	ft_putstr_fd("minishell: cd: ", 2);
	perror(path);
	if (shell == NULL)
		return (EXIT_FAILURE);
	ft_putstr_fd("error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n", 2);
	ft_set_env(shell->env, "PWD", path);
	return (EXIT_FAILURE);
}

int	ft_cd(t_token *token, t_shell *shell)
{
	char	pwd[PATH_MAX];
	char	*path;

	token->args[1] = ft_path(token->args[1], shell);
	path = token->args[1];
	if (path == NULL)
		return (EXIT_FAILURE);
	if (token->args[1] == NULL && !ft_first_condition(shell))
		return (EXIT_SUCCESS);
	else if (token->args[1] == NULL)
		return (EXIT_FAILURE);
	else if (!ft_strncmp(token->args[1], "-\0", 2))
		return (ft_second_condition(shell, pwd));
	else
	{
		if (chdir(path))
			return (ft_cd_error(path, NULL));
		if (getcwd(pwd, PATH_MAX) == NULL)
			return (ft_cd_error(path, shell));
		if (ft_set_env(shell->env, "OLDPWD", ft_getenv("PWD", shell->env)) || \
			ft_set_env(shell->env, "PWD", pwd))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
