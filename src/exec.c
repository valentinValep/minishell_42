#include "minishell.h"
#include <stdlib.h>

int	exec(t_command cmd, t_context *context)
{
	const t_builtin_func	built_funcs[]
		= {ft_echo, ft_cd, ft_pwd, ft_export, ft_unset, ft_env, ft_exit};
	const char				*built_str[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	char					*old_xpath;
	int						i;

	// DEV INFO
	printf("Command <%s>, Args <[", cmd.cmd);
	for (size_t j = 0; cmd.args && cmd.args[j]; j++)
	{
		printf("\"%s\"", cmd.args[j]);
		if (cmd.args[j + 1]) printf(", ");
	}
	printf("]>\n##########Output##########\n");
	// END DEV INFO
	old_xpath = get_env_value(&context->env, "_");
	add_env(&context->env, "_", cmd.cmd); // @TODO use complete path exept for builtins
	i = 0;
	while (i < 7)
	{
		if (ft_lineeq(cmd.cmd, built_str[i]))
			break ;
		i++;
	}
	if (i < 7)
		built_funcs[i](cmd.args, context);
	else
		printf("Command is not builtin <%s>\n", cmd.cmd);
	add_env(&context->env, "_", old_xpath);
	free(old_xpath);
	// @TODO free args and *args
	return (0);
}
