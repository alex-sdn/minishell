#include "minishell.h"

static int	get_exit_status(char **cmds)
{
	int i;
	int status;

	i = -1;
	if (cmds[1][0] == '-')
		i++;
	while (cmds[1][++i])
		if (ft_isdigit(cmds[1][i]) == 0)
			return (printf_error(ERR_EXIT_1, cmds[1]), 2);  //and if i > 20
	status = ft_atoi(cmds[1]);  //FT_ATOLL !!!!!
	if (cmds[2])
		return (printf_error(ERR_EXIT_2, NULL), -1);
	while (status > 256)
			status -= 256;
	while (status < 0)
		status = 256 + status;
	return (status);
}

int	ft_exit(t_list **env, t_cmd_lst **cmd_lst, int status, int fake)
{
	if (!fake)
		printf("exit\n");
	if (cmd_lst && ft_strncmp("exit", (*cmd_lst)->cmds[0], 4) == 0 && (*cmd_lst)->cmds[1])
		status = get_exit_status((*cmd_lst)->cmds);
	if (status >= 0)
	{
		ft_lstclear(env, &ft_del);
		rl_clear_history();
		if (cmd_lst)
			free_cmd_lst(cmd_lst);
		exit(status);
	}
	return (1);
}
