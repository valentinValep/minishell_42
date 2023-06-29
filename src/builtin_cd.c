
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int	is_in_export(t_context *context, char *key)
{
	int	i;

	if (get_env_offset(&context->env, key) != -1)
		return (1);
	i = 0;
	while (i < context->export.len)
	{
		if (ft_streq(((char **)context->export.tab)[i], key))
			return (1);
		i++;
	}
	return (0);
}

static int	auto_path(char **path, t_context *context)
{
	if (!*path)
	{
		*path = get_env_value(&context->env, "HOME");
		if (!*path)
			return (printf_fd(STDERR_FILENO,
					"minishell: cd: HOME not set\n"), 1);
	}
	return (0);
}

static int	precedent_path(char **path, int *print_path, t_context *context)
{
	if (ft_streq(*path, "-"))
	{
		*print_path = 1;
		if (!context->oldpwd)
			return (printf_fd(STDERR_FILENO,
					"minishell: cd: OLDPWD not set\n"), 1);
		*path = context->oldpwd;
	}
	else
		*print_path = 0;
	return (0);
}

static void	manage_oldpwd(t_context *context)
{
	unset("OLDPWD", context, EXPORT | ENV);
	add_export("OLDPWD", context);
}

int	cd(char *path, t_context *context)
{
	int	print_path;

	if (auto_path(&path, context))
		return (1);
	if (precedent_path(&path, &print_path, context))
		return (1);
	if (chdir(path))
		return (
			printf_fd(STDERR_FILENO,
				"minishell: cd: %s: %s\n", path, strerror(errno)), 1);
	free_node(context->oldpwd);
	context->oldpwd = context->pwd;
	if (is_in_export(context, "OLDPWD"))
		manage_oldpwd(context);
	context->pwd = getcwd(NULL, 0);
	add_node(context->pwd);
	if (is_in_export(context, "PWD"))
	{
		unset("PWD", context, EXPORT);
		add_env(&context->env, "PWD", context->pwd);
	}
	if (print_path)
		printf_fd(STDOUT_FILENO, "%s\n", context->pwd);
	return (0);
}
