#include "minishell.h"

// essaie d'ouvrir les fd, print error si pb
// ✅ pas tout a fait finalement
int	open_files(t_parsed_lst **cmd_lst)  //en theorie devrait verifier in/out dans l'ordre du prompt..
{
	t_parsed_lst	*tmp;

	tmp = *cmd_lst;
	while (tmp)
	{
		if (tmp->in_file)
		{
			tmp->in_fd = open(tmp->in_file, O_RDONLY);
			if (tmp->in_fd < 0)
				printf("%s: %s\n", tmp->in_file, strerror(errno));
		}
		if (tmp->out_file && tmp->in_fd >= 0)
		{
			if (tmp->out_mode == 0)
				tmp->out_fd = open(tmp->out_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
			else
				tmp->out_fd = open(tmp->out_file, O_CREAT | O_RDWR | O_APPEND, 0644);  // doit ouvrir dans child proc
			if (tmp->out_fd < 0)
				printf("%s: %s\n", tmp->out_file, strerror(errno));
		}
		tmp = tmp->next;
	}
	return (errno);
}

// verifie que cmd est executable (uniquement si fd sont ok)
// ✅
int	access_loop(char *cmd, char **paths)
{
	char	*pathcmd;
	int		i;

	i = 0;
	if (is_builtin(cmd) > 0)
		return (0);
	while (paths[i])
	{
		pathcmd = ft_strjoin(paths[i], cmd);
		if (!pathcmd)
			return (-1);
		if (access(pathcmd, X_OK) == 0)
			break ;
		free(pathcmd);
		i++;
	}
	if (!paths[i])
		printf("%s: command not found\n", cmd);
	else
		free(pathcmd);
	return (0);
}

// ✅
int	check_access(t_parsed_lst *cmd_lst, t_list **env)
{
	char	**paths;
	int		i;
	
	i = 0;
	paths = get_paths(env);
	if (!paths)
		return (-1); // malloc error //peut meme peut etre exit si tous les malloc sont own
	while (cmd_lst)
	{
		if (cmd_lst->in_fd >= 0 && cmd_lst->out_fd >= 0)
			if (access_loop(cmd_lst->cmds[0], paths) < 0)
				return (free_tab(paths), -1);  //malloc error
		cmd_lst = cmd_lst->next;
		i++;
	}
	return (free_tab(paths), errno);
}

// trouve comment gerer $? -> resultat de la derniere commande (0 ok, 127 pas ok, 1 err ficher...)
int	exec_main(t_parsed_lst **cmd_lst, t_list **env, int cmd_count)
{
	int		last;   // pour $?
	int 	status = 0;
	pid_t	process_id;

	// memset(&in_file, 0, sizeof(int) * cmd_count);
	// memset(&out_file, 0, sizeof(int) * cmd_count);  sert plus a rien normalement

	if (cmd_count == 1)
		replace_env(env, "_", get_last_str((*cmd_lst)->cmds));

	last = open_files(cmd_lst);
	last += check_access(*cmd_lst, env);

	//fork avant ? un process pour lancer le loop d'exec, un autre pour continuer le reste du minishell
	process_id = fork();
	if (process_id < 0)
		return (-1);  //error fork!
	if (process_id == 0)  //conditions supplementaires ?
		exec_cmds(*cmd_lst, env);
	else
		wait(&status); //besoin d'un int surement
	
	// return(last)
	return (status); // if status == 0 return last ? pour $?	
}
