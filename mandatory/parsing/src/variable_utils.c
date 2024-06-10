/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:20:34 by zouddach          #+#    #+#             */
/*   Updated: 2024/06/10 21:37:59 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	ft_arg_len(char *str)
{
	int	len;

	len = 0;
	while (*str && !ft_whitespace(*str))
	{
		if (*str == '\"')
			while (*++str && *str != '\"')
				len++;
		else if (*str == '\'')
			while (*++str && *str != '\'')
				len++;
		else
			len++;
		if (*str)
			str++;
	}
	return (len);
}

char	*ft_remove_quotes(char *str)
{
	char	*word;
	int		i;

	i = 0;
	word = (char *)malloc(sizeof(char) * (ft_arg_len(str) + 1));
	if (!word)
		return (NULL);
	while (*str && !ft_whitespace(*str))
	{
		if (*str == '\"')
			while (*++str && *str != '\"')
				word[i++] = *str;
		else if (*str == '\'')
			while (*++str && *str != '\'')
				word[i++] = *str;
		else
			word[i++] = *str;
		str++;
	}
	word[i] = '\0';
	return (word);
}

char	*ft_handle_tilde(char *tilde, t_shell *shell)
{
	char	*home;
	char	*new;

	if (tilde[1] == '/' || tilde[1] == '\0')
	{
		home = ft_getenv("HOME", shell->env);
		if (!home)
			return (NULL);
		new = ft_strjoin(home, tilde + 1);
		if (!new)
			return (NULL);
		free(tilde);
		return (new);
	}
	return (tilde);
}

// if (token->left)
// 	ft_expand(token->left, shell);
// if (token->right)
// 	ft_expand(token->right, shell);
// if (token->type == WORD)
// }
// {

int	ft_expand(t_token *token, t_shell *shell)
{
	char	**old_args;
	int		i;

	i = 0;
	if (!ft_strncmp(token->args[0], "export\0", 7))
		return (0);
	old_args = token->args;
	token->args = NULL;
	while (old_args[i])
	{
		if (ft_contains_variable(old_args[i]))
			ft_variables(old_args[i], token, shell);
		else if (ft_found_token(old_args[i], '*'))
			ft_wildcard(old_args[i], token, shell);
		else
		{
			if (old_args[i][0] == '~')
				old_args[i] = ft_handle_tilde(old_args[i], shell);
			old_args[i] = ft_remove_quotes(old_args[i]);
			token->args = ft_append_to_array(token->args, old_args[i]);
		}
		i++;
	}
	return (0);
}
