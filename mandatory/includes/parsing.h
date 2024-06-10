/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 10:01:47 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/06/10 21:38:30 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h>
# include "minishell.h"
# include "readline/readline.h"
# include "readline/history.h"

int			ft_parse(char *line, t_shell *shell);
int			ft_stage_and(char *str, t_token **token);
int			ft_word_len(char *str);
char		*ft_merge(char *part_one, int l_1, char *part_two, int l_2);
char		**ft_cmd_split(char *str);
int			ft_skip_parentheses(char *str);
int			ft_reserved_word(char *str);
int			ft_throw_syntax_error(char *word);
void		ft_free(char **ptr);

t_token		*ft_add_token(t_type type, t_token **token);
int			ft_skip_parentheses(char *str);
int			ft_stage_four(char *str, int end, t_token **token);
int			ft_stage_three(char *str, int end, t_token **token);
int			ft_stage_two(char *str, int end, t_token **token);
int			ft_stage_or(char *str, int end, t_token **token);
int			ft_stage_and(char *str, t_token **token);

char    	**ft_append_to_array(char **args, char *new_arg);

/*A function that expand variables to there values from the env*/
int			ft_variables(char *str, t_token *token, t_shell *shell);

/*A function that changes the '*' to a dir content*/
char		*ft_wildcard(char *pattern, t_token *token, t_shell *shell);

int			ft_variable_length(char *str);

char		*ft_wildcard_match(char *pattern, char *str, t_token *token, t_shell *shell);

char	*ft_handle_wildcard(char *pattern, int *i, char **str, t_shell *shell);

char	*ft_single_quotes_wildcard(char *pattern, int i);

char		*ft_quoted_variables(char *str, t_shell *shell);

char		*ft_split_variable(char *str, char *new, t_token *token, t_shell *shell);

char		*ft_remove_quotes(char *str);

#endif