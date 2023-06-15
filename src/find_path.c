/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlepille <vlepille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:19:28 by aboulest          #+#    #+#             */
/*   Updated: 2023/06/15 16:21:18 by vlepille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <string.h>

// @todo moddifier les mallocs !!
// @todo tester les mallocs !!

void	free_db_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	if (tab)
		free(tab);
}

char	**ft_path(t_env *env)
{
	char	*path;
	char	**final_path;

	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	final_path = ft_split(path, ':');
	free_node(path);
	return (final_path);
}

char	*find_path_in_env(char *command, t_env *env)
{
	char	*final_path;
	char	**path;
	char	*add_slash;
	int		i;

	path = ft_path(env);
	if (!path)
		return (NULL);
	i = -1;
	while (path[++i])
	{
		add_slash = ft_strjoin(path[i], "/");
		final_path = ft_strjoin(add_slash, command);
		free(add_slash);
		if (access(final_path, F_OK) == 0)
			return (free_db_tab(path), final_path);
		if (final_path)
			free(final_path);
	}
	free_db_tab(path);
	return (NULL);
}

char	*find_path(char *command, t_context *context)
{
	if (command == NULL || command[0] == '\0')
		return (NULL);
	if (ft_strchr(command, '/') != 0)
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		else if (access(command, F_OK) == 0)
		{
			printf_fd(
				STDERR_FILENO, "minishell: %s: %s\n", command, strerror(errno));
			context->exit_value = 126;
			return (ft_strdup(command));
		}
		else
		{
			printf_fd(
				STDERR_FILENO, "minishell: %s: %s\n", command, strerror(errno));
			context->exit_value = 127;
		}
	}
	return (find_path_in_env(command, &context->env));
}
