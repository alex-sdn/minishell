#include "minishell.h"

int	ft_pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
		return (1);
	ft_putstr_fd(buf, 1);
	ft_putchar_fd('\n', 1);
	free(buf);
	return (0);
}
