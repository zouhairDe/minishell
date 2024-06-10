/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 20:59:05 by zouddach          #+#    #+#             */
/*   Updated: 2024/06/10 18:36:12 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	ft_dup_pipes(int fdin, int fdout)
{
	if (fdin != 0)
	{
		dup2(fdin, 0);
		close(fdin);
	}
	if (fdout != 1)
	{
		dup2(fdout, 1);
		close(fdout);
	}
}

int	ft_perror(char *cmd)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": permission denied\n", 2);
		return (126);
	}
	else
		perror(cmd);
	return (errno);
}

void	ft_increment_shellvl(t_shell *shell)
{
	char	*shell_lvl;
	int		shell_lvl_int;

	shell_lvl = ft_getenv("SHLVL", shell->env);
	if (!shell_lvl)
		return ;
	shell_lvl_int = ft_atoi(shell_lvl);
	shell_lvl_int++;
	shell_lvl = ft_itoa(shell_lvl_int);
	if (!shell_lvl)
		return ;
	ft_set_env(shell->env, "SHLVL", shell_lvl);
	free(shell_lvl);
}

char	**ft_list_to_array(t_list *env)
{
	char	**env_array;
	t_list	*tmp;
	int		i;

	i = 0;
	env_array = (char **)malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	if (!env_array)
		return (NULL);
	while (env)
	{
		tmp = env;
		env_array[i] = ft_strdup(env->content);
		if (!env_array[i])
			return (NULL);
		env = env->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	ft_enable_echoctl(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
