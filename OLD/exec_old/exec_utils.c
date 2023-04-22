#include "minishell.h"

//mettre dans cmdlst_utils plutot
int	parsed_lst_size(t_parsed_lst *cmd_lst)
{
	int	count;

	count = 0;
	if (cmd_lst)
		count++;
	while (cmd_lst->next)
	{
		count++;
		cmd_lst = cmd_lst->next;
	}
	return (count);
}

//returns char** with all PATHS in the env
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

//creates char** env from t_list *env
char	**create_env_tab(t_list *env)
{
	int		i;
	int		size;
	char	**dest;

	i = 0;
	size = ft_lstsize(env);
	dest = malloc(sizeof(char *) * size + 1);
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

//returns last str from a tab
char	*get_last_str(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (tab[i - 1]);
}

//returns which builtin it is, or 0 if not a builtin
int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0 && ft_strlen(cmd) == 2)
		return (1);
	if (ft_strncmp(cmd, "echo", 4) == 0 && ft_strlen(cmd) == 4)
		return (2);
	if (ft_strncmp(cmd, "env", 3) == 0 && ft_strlen(cmd) == 3)
		return (3);
	if (ft_strncmp(cmd, "exit", 4) == 0 && ft_strlen(cmd) == 4)
		return (4);
	if (ft_strncmp(cmd, "export", 6) == 0 && ft_strlen(cmd) == 6)
		return (5);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && ft_strlen(cmd) == 3)
		return (6);
	if (ft_strncmp(cmd, "unset", 5) == 0 && ft_strlen(cmd) == 5)
		return (7);
	return (0);
}

//executes builtin function
void	exec_builtin(t_parsed_lst *cmd_lst, t_list **env)
{
	int	builtin_id;
	int	status;

	builtin_id = is_builtin(cmd_lst->cmds[0]);
	if (builtin_id == FT_CD)
		status = ft_cd(env, cmd_lst);
	else if (builtin_id == FT_ECHO)
		status = ft_echo(cmd_lst);
	else if (builtin_id == FT_ENV)
		status = ft_env(*env, cmd_lst);
	// else if (builtin_id == FT_EXIT)
	// 	status = ft_exit(env, cmd_lst); //besoin de **cmd_lst ?? //doit lancer avant exec loop ?
	if (builtin_id == FT_EXPORT)
		status = ft_export(env, cmd_lst);
	else if (builtin_id == FT_PWD)
		status = ft_pwd(cmd_lst);
	else if (builtin_id == FT_UNSET)
		status = ft_unset(env, cmd_lst);
	exit(status);
}
