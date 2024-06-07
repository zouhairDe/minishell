/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 05:18:51 by zouddach          #+#    #+#             */
/*   Updated: 2024/06/07 19:43:56 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_execution_process(t_token *token, int fdin, int fdout, t_shell *shell)
{
	char	**env_array;
	char	*cmd_path;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		ft_enable_echoctl();
		if (fdin < 0)
			exit(EXIT_FAILURE);
		ft_dup_pipes(fdin, fdout);
		env_array = ft_list_to_array(shell->env);
		cmd_path = ft_allocate_cmd(token->args, env_array);
		if (!cmd_path)
			return (EXIT_FAILURE);
		execve(cmd_path, token->args, env_array);
		exit(ft_perror(token->args[0]));
	}
	else if (pid < 0)
		return (ft_perror("fork"));
	return (EXIT_SUCCESS);
}

void	sig2_handler(int signum)
{
	if (signum == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", 2);
	}
	else if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 1);
	}
}

int	ft_handle_dots(t_token *token)
{
	if (ft_strncmp(token->args[0], ".\0", 2) == 0)
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		return (2);
	}
	else
	{
		ft_putstr_fd("minishell: ..: command not found\n", 2);
		return (127);
	}
}

int	ft_exec_function(t_token *token, int fdin, int fdout, t_shell *shell)
{
	char	*last_cmd;

	if (!token || token->args == NULL)
		return (EXIT_FAILURE);
	if (ft_expand(token, shell) || token->args == NULL)
		return (EXIT_FAILURE);
	if (ft_strncmp(token->args[0], ".\0", 2) == 0 || \
		ft_strncmp(token->args[0], "..\0", 3) == 0)
		return (ft_handle_dots(token));
	if (ft_is_builtin(token))
		return (ft_execute_builtin(token, fdout, shell));
	if (ft_strncmp(token->args[0], "./minishell\0", 12) == 0)
		ft_increment_shellvl(shell);
	signal(SIGINT, &sig2_handler);
	signal(SIGQUIT, &sig2_handler);
	if (ft_execution_process(token, fdin, fdout, shell))
		return (EXIT_FAILURE);
	last_cmd = token->args[ft_array_len(token->args) - 1];
	if (ft_set_env(shell->env, "_=", last_cmd))
		return (EXIT_FAILURE);
	return (0);
}