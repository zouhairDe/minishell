/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:29:36 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/05/09 23:55:16 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// use recursion for parantheses

int	ft_whitespace(char c)
{
	return (c == ' ' || c == 127 || (c >= 9 && c <= 13));
}

// char	*ft_cp_word(char *str)
// {
// 	char	*word;
// 	int		len;
// 	int		i;

// 	i = 0;
// 	len = 0;
// 	while (str[len] && ft_whitespace(str[len]))
// 		len++;
// 	while (str[len] && !ft_whitespace(str[len]))
// 		len++;
// 	word = (char *)malloc(sizeof(char) * (len + 1));
// 	while (str[len] && !ft_whitespace(str[i]))
// 	{
// 		word[i] = str[i];
// 		i++;
// 	}
// 	word[i] = '\0';
// 	return (word);
// }

// int	ft_word_len(char *str)
// {
// 	int	len;

// 	len = 0;
// 	while (str[len] && !ft_whitespace(str[len]))
// 		len++;
// 	return (len);
// }

// t_token	*ft_get_latest_token(t_env *env)
// {
// 	t_token	*token;

// 	if (!env->root)
// 	{
// 		env->root = (t_token *)malloc(sizeof(t_token));
// 		token = env->root;
// 	}
// 	else
// 	{
// 		token = env->root;
// 		while (token->left)
// 			token = token->left;
// 		token->left = (t_token *)malloc(sizeof(t_token));
// 		token = token->left;
// 	}
// 	token->right = NULL;
// 	token->left = NULL;
// 	return (token);
// }

// int	ft_parse_redir_in(char *line, t_env *env)
// {
// 	t_token	*token;
// 	int		len;
// 	int		i;

// 	i = 1;
// 	token = ft_get_latest_token(env);
// 	token->type = REDIR_IN;
// 	token->input = 0;
// 	token->output = 1;
// 	token->args = (char **)malloc(sizeof(char *) * 3);
// 	token->args[2] = NULL;
// 	if (line[i] == '<')
// 		token->args[0 * i++] = "<<";
// 	else
// 		token->args[0] = "<";
// 	while (line[i] && ft_whitespace(line[i]))
// 		i++;
// 	len = ft_word_len(&line[i]);
// 	token->args[1] = (char *)malloc(sizeof(char) * (len + 1));
// 	ft_strlcpy(token->args[1], &line[i], len + 1);
// 	return (len + i);
// }

// int	ft_parse_redir_out(char *line, t_env *env)
// {
// 	t_token	*token;
// 	int		len;
// 	int		i;

// 	i = 1;
// 	token = ft_get_latest_token(env);
// 	token->type = REDIR_OUT;
// 	token->input = 0;
// 	token->output = 1;
// 	token->args = (char **)malloc(sizeof(char *) * 3);
// 	token->args[2] = NULL;
// 	if (line[i] == '>')
// 		token->args[0 * i++] = ">>";
// 	else
// 		token->args[0] = ">";
// 	while (line[i] && ft_whitespace(line[i]))
// 		i++;
// 	len = ft_word_len(&line[i]);
// 	token->args[1] = (char *)malloc(sizeof(char) * (len + 1));
// 	ft_strlcpy(token->args[1], &line[i], len + 1);
// 	return (len + i);
// }

void	ft_print_tree(t_token *token)
{
	while (token)
	{
		if (token->type == WORD)
			printf("WORD: %s\n", token->args[0]);
		else if (token->type == PIPE)
			printf("PIPE\n");
		else if (token->type == AND)
			printf("AND\n");
		else if (token->type == OR)
			printf("OR\n");
		if (token->left)
			ft_print_tree(token->left);
		token = token->right;
	}
}

int	ft_parse(char *line, t_shell *shell)
{
	int		i;

	i = 0;
	shell->root = NULL;
	ft_stage_one(line, &shell->root);
	ft_print_tree(shell->root);
	// while (line[i])
	// {
	// 	if (line[i] == '<')
	// 		i += ft_parse_redir_in(&line[i], env);
	// 	else if (line[i] == '>')
	// 		i += ft_parse_redir_out(&line[i], env);
	// 	else if (line[i] == '|')
	// 		i = ft_parse_pipe(&line[i], env);
	// 	else
	// 		i++;
	// }
	// i = 0;
	// t_token *token = env->root;
	// while (token)
	// {
	// 	i = 0;
	// 	while (token->args[i])
	// 		printf("args: %s\n", token->args[i++]);
	// 	token = token->left;
	// }
}
