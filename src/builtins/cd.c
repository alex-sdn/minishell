/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:13:59 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:14:01 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_cdcat(char *curr, char *new)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (curr[i])
		i++;
	curr[i++] = '/';
	while (new[j])
		curr[i++] = new[j++];
	curr[i] = '\0';
	return (curr);
}

static int	cd_success(t_list **env, char *old_path, char *new_path, int type)
{
	replace_env(env, "OLDPWD", old_path);
	free(old_path);
	replace_env(env, "PWD", new_path);
	if (type == 2)
		free(new_path);
	return (0);
}

static int	cd_dotdot(t_list **env, t_cmd_lst *cmd_lst)
{
	char	*old_path;
	char	*new_path;
	int		new_len;

	old_path = getcwd(NULL, 0);
	new_len = ft_strlen(old_path);
	while (old_path[new_len] != '/')
		new_len--;
	new_path = malloc(sizeof(char) * (new_len + 1));
	if (!new_path)
		return (free(old_path), 1);
	new_path[new_len] = '\0';
	while (--new_len >= 0)
		new_path[new_len] = old_path[new_len];
	if (chdir(new_path) == 0)
		return (cd_success(env, old_path, new_path, 2));
	return (free(new_path), free(old_path), perror(cmd_lst->cmds[1]), 1);
}

static int	cd_relative(t_list **env, t_cmd_lst *cmd_lst)
{
	char	*new_path;
	char	*old_path;
	int		len;

	if (cmd_lst->cmds[1][0] == '.' && ft_strlen(cmd_lst->cmds[1]) == 1)
	{
		new_path = ft_getenv(*env, "PWD");
		if (!new_path)
			return (1);
		replace_env(env, "OLDPWD", new_path);
		return (free(new_path), 0);
	}
	if (ft_strncmp(cmd_lst->cmds[1], "..", 2) == 0
		&& ft_strlen(cmd_lst->cmds[1]) == 2)
		return (cd_dotdot(env, cmd_lst));
	old_path = getcwd(NULL, 0);
	len = ft_strlen(old_path);
	new_path = malloc(sizeof(char) * (len + ft_strlen(cmd_lst->cmds[1]) + 2));
	if (!new_path)
		return (free(old_path), 1);
	new_path = getcwd(new_path, len + 2);
	new_path = ft_cdcat(new_path, cmd_lst->cmds[1]);
	if (chdir(new_path) == 0)
		return (cd_success(env, old_path, new_path, 2));
	return (free(new_path), free(old_path), perror(cmd_lst->cmds[1]), 1);
}

int	ft_cd(t_list **env, t_cmd_lst *cmds)
{
	char	*new_path;
	char	*old_p;

	if (!cmds->cmds[1] || (cmds->cmds[1][0] == '~' && !cmds->cmds[1][1]))
	{
		old_p = getcwd(NULL, 0);
		new_path = ft_getenv(*env, "HOME");
		if (!new_path)
			return (1);
		if (ft_strlen(new_path) == 0)
			return (free(old_p), free(new_path), printf_error(ERR_HOME, 0), 1);
		if (chdir(new_path) == 0)
			return (cd_success(env, old_p, new_path, 2));
		return (free(old_p), perror(new_path), free(new_path), 1);
	}
	if (cmds->cmds[2])
		return (write(2, "cd: too many arguments\n", 23), 1);
	if (cmds->cmds[1] && cmds->cmds[1][0] == '/')
	{
		old_p = getcwd(NULL, 0);
		if (chdir(cmds->cmds[1]) == 0)
			return (cd_success(env, old_p, cmds->cmds[1], 1));
		return (free(old_p), perror(cmds->cmds[1]), 1);
	}
	return (cd_relative(env, cmds));
}
