/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_three.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouddach <zouddach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 23:18:26 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/06/09 16:16:49 by zouddach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_throw_syntax_error(char *word)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(word, 2);
	ft_putstr_fd("`.\n", 2);
	return (PARSING_FAILURE);
}

int	ft_skip_parentheses(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '(')
		i++;
	while (str[i])
	{
		if (!ft_strncmp(&str[i], "\"", 1))
			i += ft_index(&str[i + 1], '\"') + 1;
		else if (!ft_strncmp(&str[i], "\'", 1))
			i += ft_index(&str[i + 1], '\'') + 1;
		else if (!ft_strncmp(&str[i], "(", 1))
			i += ft_skip_parentheses(&str[i]) + 1;
		else if (!ft_strncmp(&str[i], ")", 1))
			return (i);
		i++;
	}
	return (i);
}

int	ft_parse_word(char *str, int end, t_token **token)
{
	char	*word;
	int		p;

	p = 0;
	if (!ft_add_token(WORD, token))
		return (EXIT_FAILURE);
	while (p < end)
	{
		if (!ft_strncmp(&str[p], "\"", 1))
			p += ft_index(&str[p + 1], '\"') + 1;
		else if (!ft_strncmp(&str[p], "\'", 1))
			p += ft_index(&str[p + 1], '\'') + 1;
		else if (str[p] == '(')
			return (ft_throw_syntax_error(&str[p]));
		p++;
	}
	word = ft_substr(str, 0, end);
	if (!word)
		return (EXIT_FAILURE);
	(*token)->args = ft_cmd_split(word);
	if ((*token)->args == NULL)
		return (EXIT_FAILURE);
	free(word);
	return (EXIT_SUCCESS);
}

t_token	*ft_add_token(t_type type, t_token **token)
{
	t_token	*new;

	if (*token)
		return (*token);
	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->left = NULL;
	new->right = NULL;
	new->args = NULL;
	*token = new;
	return (new);
}

int	ft_handle_parentheses(char *str, int end, t_token **token)
{
	char	*word;
	int		p;
	int		i;

	i = 1;
	if (!ft_add_token(SUBSHELL, token))
		return (EXIT_FAILURE);
	p = ft_skip_parentheses(str) + 1;
	while (i < (p - 1) && ft_whitespace(str[i]))
		i++;
	if (i == (p - 1))
		return (ft_throw_syntax_error(&str[i]));
	while (p < end)
	{
		if (!ft_whitespace(str[p]))
			return (ft_throw_syntax_error(&str[p]));
		p++;
	}
	p = ft_skip_parentheses(str);
	word = ft_substr(str, 1, p - 1);
	ft_stage_and(word, &(*token)->right);
	return (EXIT_SUCCESS);
}
