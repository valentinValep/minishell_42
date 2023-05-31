
#include "minishell.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int	open_infile(char *file)
{
	if (access(file, F_OK) != 0)
	{
		perror(file);
		return (-1);
	}
	else if (access(file, R_OK) == 0)
		return (open(file, O_RDONLY));
	else
	{
		perror(file);
		return (-1);
	}
}

int	open_outfile(char *file)
{
	int	fd;

	if (access(file, F_OK) != 0)
	{
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			perror(file);
		return (fd);
	}
	else if (access(file, W_OK) == 0)
		return (open(file, O_WRONLY | O_TRUNC));
	else
	{
		perror(file);
		return (-1);
	}
}

int	open_outfile_extend(char *file)
{
	int	fd;

	if (access(file, F_OK) != 0)
	{
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			perror(file);
		return (fd);
	}
	else if (access(file, W_OK) == 0)
		return (open(file, O_WRONLY | O_APPEND));
	else
	{
		perror(file);
		return (-1);
	}
}