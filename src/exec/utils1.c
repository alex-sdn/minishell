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
	else if (status == 3)
		return (131);
	else if (status == 13)
		return (126);
	else if (status == 512)
		return (2);
	else if (status == 256)
		return (1);
	return (status);
}
