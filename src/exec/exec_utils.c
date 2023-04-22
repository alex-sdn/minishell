#include "minishell.h"

char	**get_paths(t_list **env)
{
	char	**paths;
	t_list	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp("PATH=", (char *)tmp->content, 5) == 0)
			break ;
		tmp = tmp->next;
	}
	if (tmp)
		paths = ft_split_pipex((char *)tmp->content + 5, ':');
	else
	{
		paths = malloc(sizeof(char *) * 2);
		if (!paths)
			return (NULL);
		paths[0] = malloc(sizeof(char) * 1);
		if (!paths[0])
			return (free(paths), NULL);
		paths[0][0] = '\0';
		paths[1] = 0;
	}
	return (paths);
}

char	**create_env_tab(t_list *env)
{
	int		i;
	int		size;
	char	**dest;

	i = 0;
	size = ft_lstsize(env);
	dest = malloc(sizeof(char *) * (size + 1));
	if (!dest)
		return (NULL);
	while (i < size)
	{
		dest[i] = (char *)env->content;
		env = env->next;
		i++;
	}
	dest[i] = 0;
	return (dest);
}

char	*get_last_str(char **tab)
{
	int	i;

	i = 0;
	if (!tab[0])
		return (NULL);
	while (tab[i])
		i++;
	return (tab[i - 1]);
}

int	convert_status(int status)
{
	if (status == 2)
		return (127);
	else if (status == 13)
		return (126);
	else if (status == 512)
		return (2);
	else if (status == 256)
		return (1);
	return (status);
}

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0 && ft_strlen(cmd) == 2)
		return (FT_CD);
	if (ft_strncmp(cmd, "echo", 4) == 0 && ft_strlen(cmd) == 4)
		return (FT_ECHO);
	if (ft_strncmp(cmd, "env", 3) == 0 && ft_strlen(cmd) == 3)
		return (FT_ENV);
	if (ft_strncmp(cmd, "exit", 4) == 0 && ft_strlen(cmd) == 4)
		return (FT_EXIT);
	if (ft_strncmp(cmd, "export", 6) == 0 && ft_strlen(cmd) == 6)
		return (FT_EXPORT);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && ft_strlen(cmd) == 3)
		return (FT_PWD);
	if (ft_strncmp(cmd, "unset", 5) == 0 && ft_strlen(cmd) == 5)
		return (FT_UNSET);
	return (0);
}
