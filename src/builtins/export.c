#include "minishell.h"

static int	is_valid_id(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < 48 || (str[i] > 57 && str[i] < 65)
			|| (str[i] > 90 && str[i] < 95) || str[i] == 96 || str[i] > 122)
			return (1);
		i++;
	}
	return (0);
}

int	ft_export(t_list **env, t_cmd_lst *cmd_lst)
{
	char	**args_split;
	int		i;
	int		status;

	i = 0;
	status = 0;
	if (!cmd_lst->cmds[1])
		return (ft_env(*env, cmd_lst), 0);
	while (cmd_lst->cmds[++i])
	{
		args_split = ft_split(cmd_lst->cmds[i], '=');
		if (!args_split || !args_split[0])
			return (1);
		if (is_valid_id(args_split[0]) == 1 || (ft_isalpha(args_split[0][0]) == 0 && args_split[0][0] != '_'))
		{
			printf_error(ERR_EXPORT, cmd_lst->cmds[i]);
			status = 1;
		}
		else if (args_split[1])
			if (replace_env(env, args_split[0], args_split[1]) == 1)
				if (args_split[1] && add_env(env, cmd_lst->cmds[i]) == 1)
					return (free_tab(args_split), 1);
		free_tab(args_split);
	}
	return (status);
}
