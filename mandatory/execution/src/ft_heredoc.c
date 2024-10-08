/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 05:27:06 by zouddach          #+#    #+#             */
/*   Updated: 2024/07/11 19:13:23 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	sig_herdoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		close(0);
		g_signal = SIGINT;
	}
	return ;
}

int	ft_heredoc_content(int start, int stdin_copy)
{
	if (g_signal == SIGINT)
	{
		dup2(stdin_copy, 0);
		close(stdin_copy);
		close(start);
		signal(SIGINT, &main_sig_handler);
		g_signal = 0;
		return (-1);
	}
	signal(SIGINT, &main_sig_handler);
	close(stdin_copy);
	return (start);
}

char	*ft_heredoc_variables(char *str, t_shell *shell)
{
	char	*tmp;
	char	*new;

	free(str);
	new = NULL;
	while (*str)
	{
		if (*str == '$' && (ft_isalnum(*(str + 1)) || \
			ft_strchr("?_", *(str + 1))))
		{
			tmp = ft_expand_variable(++str, shell);
			new = ft_realloc(new, tmp);
			free(tmp);
			str += ft_variable_length(str);
		}
		else
		{
			tmp = ft_substr(str, 0, 1);
			new = ft_realloc(new, tmp);
			free(tmp);
			str++;
		}
	}
	return (new);
}

void	ft_heredoc_loop(char *delimiter, int heredoc_file, t_shell *shell)
{
	char	*buffer;

	buffer = NULL;
	while (true)
	{
		buffer = readline("> ");
		if (!buffer)
			break ;
		if (ft_contains_variable(buffer))
			buffer = ft_heredoc_variables(buffer, shell);
		if (!ft_strncmp(buffer, delimiter, ft_strlen(delimiter) + 1))
			break ;
		ft_putstr_fd(buffer, heredoc_file);
		ft_putstr_fd("\n", heredoc_file);
		free(buffer);
	}
	free(buffer);
}

int	ft_redir_heredoc_function(t_token *token, t_shell *shell)
{
	int		heredoc_file;
	int		stdin_copy;
	int		start;

	g_signal = 0;
	stdin_copy = dup(0);
	heredoc_file = open("/tmp/heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	start = open("/tmp/heredoc", O_RDONLY);
	unlink("/tmp/heredoc");
	signal(SIGINT, sig_herdoc_handler);
	ft_heredoc_loop(token->args[0], heredoc_file, shell);
	close(heredoc_file);
	return (ft_heredoc_content(start, stdin_copy));
}
