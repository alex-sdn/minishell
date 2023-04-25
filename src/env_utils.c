/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:16:04 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:16:05 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*init_env(t_list *env, char **envp)
{
	t_list	*last;
	int		i;

	i = 0;
	if (!envp)
		return (NULL);
	if (envp[i++])
	{
		env = ft_lstnew(ft_strdup(envp[0]));
		if (!env)
			return (NULL);
		if (!env->content)
			return (ft_lstclear(&env, &ft_del), NULL);
		while (envp[i])
		{
			ft_lstadd_back(&env, ft_lstnew(ft_strdup(envp[i])));
			last = ft_lstlast(env);
			if (!last->content)
				return (ft_lstclear(&env, &ft_del), NULL);
			i++;
		}
	}
	else
		env = init_env_i(env);
	return (env);
}

t_list	*init_env_i(t_list *env)
{
	char	*curr_path;
	char	*pwd;
	char	*underscore;

	curr_path = getcwd(NULL, 0);
	if (!curr_path)
		return (NULL);
	pwd = ft_strjoin("PWD=", curr_path);
	free(curr_path);
	if (!pwd)
		return (NULL);
	env = ft_lstnew(pwd);
	if (!env)
		return (free(pwd), NULL);
	underscore = ft_strdup("_=]");
	if (!underscore)
		return (ft_lstclear(&env, &ft_del), NULL);
	ft_lstadd_back(&env, ft_lstnew(underscore));
	if (!env->next)
		return (ft_lstclear(&env, &ft_del), free(underscore), NULL);
	return (env);
}

int	replace_env(t_list **env, char *env_var, char *new_content)
{
	t_list	*tmp;
	char	*full_var;
	char	*new_str;

	tmp = *env;
	while (tmp && tmp->content
		&& ft_strncmp(env_var, (char *)tmp->content, ft_strlen(env_var)) != 0)
		tmp = tmp->next;
	if (tmp && tmp->content)
	{
		full_var = ft_strjoin(env_var, "=");
		if (!full_var)
			return (-1);
		if (new_content)
			new_str = ft_strjoin(full_var, new_content);
		else
			new_str = ft_strdup(full_var);
		if (!new_str)
			return (free(full_var), -1);
		free(tmp->content);
		tmp->content = (void *)new_str;
		free(full_var);
		return (0);
	}
	return (1);
}

int	add_env(t_list **env, char *new_var)
{
	t_list	*new;
	char	*dup_content;

	dup_content = ft_strdup(new_var);
	if (!dup_content)
		return (1);
	new = ft_lstnew((void *)dup_content);
	if (!new || !new->content)
		return (free(dup_content), 1);
	ft_lstadd_back(env, new);
	return (0);
}

char	*ft_getenv(t_list *env, char *var)
{
	char	*new;
	char	*var2;

	var2 = ft_strjoin(var, "=");
	if (!var2)
		return (NULL);
	while (env && ft_strncmp(var2, (char *)env->content, ft_strlen(var2)) != 0)
		env = env->next;
	if (env && env->content)
		new = ft_strdup((char *)env->content + ft_strlen(var2));
	else
	{
		new = malloc(sizeof(char) * 1);
		if (new)
			new[0] = '\0';
	}
	free(var2);
	return (new);
}
