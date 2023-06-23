/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlepille <vlepille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 11:13:39 by aboulest          #+#    #+#             */
/*   Updated: 2023/06/20 16:39:24 by vlepille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int	g_sigint_received;

// function that return if char * is only space
int	is_only_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

void	exec_input(char *input, int len, t_context *context)
{
	t_block		main_block;

	main_block = (t_block){input, NULL, NULL, len, UNDEFINE, UNDEFINE};
	get_blocks(&main_block, context);
	exec_block(&main_block, context);
}

int	error_parsing(char *input, t_context *context)
{
	int error_par;
	int error_token;

	error_par = check(input);
	if (error_par)
	{
		context->exit_value = print_error(error_par);
		return (1);
	}
	error_token = check_error(input);
	if (error_token)
	{
		context->exit_value = print_error_token(error_token, input);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_context		context;
	char			*input;

	if (init_context(&context, argc, argv, envp))
		return (1);
	set_parent_signals();
	while (1)
	{
		if (g_sigint_received)
			input = readline("\nminishell$ ");
		else
			input = readline("minishell$ ");
		g_sigint_received = 0;
		if (!input)
			exit(context.exit_value);
		add_history(input);
		if (is_only_space(input))
		{
			free(input);
			continue ;
		}
		if (!error_parsing(input, &context))
		{
			exec_input(input, ft_strlen(input), &context);
			free(input);
		}
	}
}
