/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:19:31 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:19:32 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_next_diff(char *str, int pos)
{
	int	next_diff;

	next_diff = ft_strlen(str);
	while (str[pos] && is_special(str[pos]) == 0)
		pos++;
	next_diff -= pos;
	return (next_diff);
}

static int	has_env_var(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0' && str[i + 1] != ' ')
			return (i);
		else if (str[i] == '\'')
			i = ft_skip_quotes(str, str[i], i + 1);
		i++;
	}
	return (-1);
}

static char	*sub_env_var(char *str, int start, t_list *env)
{
	int		end;
	char	*var[2];
	char	*new;
	char	*pre_post[2];

	end = start + 1;
	while (is_special(str[end]) == 0)
		end++;
	var[0] = ft_substr(str, start + 1, end - start - 1);
	if (!var[0])
		return (NULL);
	var[1] = ft_getenv(env, var[0]);
	free(var[0]);
	if (!var[1])
		return (NULL);
	pre_post[0] = ft_substr(str, 0, start);
	pre_post[1] = ft_substr(str, end, ft_strlen(str) - end);
	if (!pre_post[0] || !pre_post[1])
		return (free(pre_post[0]), free(pre_post[1]), free(var[1]), NULL);
	new = triple_strjoin(pre_post[0], var[1], pre_post[1]);
	free(pre_post[0]);
	free(pre_post[1]);
	free(var[1]);
	return (new);
}

static char	*sub_status_var(char *str, int start, int status)
{
	int		end;
	char	*var_status;
	char	*new;
	char	*pre_post[2];

	end = start + 2;
	var_status = ft_itoa(status);
	if (!var_status)
		return (NULL);
	pre_post[0] = ft_substr(str, 0, start);
	if (!pre_post[0])
		return (free(var_status), NULL);
	pre_post[1] = ft_substr(str, end, ft_strlen(str) - end);
	if (!pre_post[1])
		return (free(pre_post[0]), free(var_status), NULL);
	new = triple_strjoin(pre_post[0], var_status, pre_post[1]);
	free(pre_post[0]);
	free(pre_post[1]);
	free(var_status);
	return (new);
}

char	*replace_env_vars(char *str, t_list *env, int status)
{
	char	*new;
	int		diff;
	int		next_diff;

	diff = 0;
	while (has_env_var(str, diff) >= 0)
	{
		next_diff = get_next_diff(str, has_env_var(str, diff) + 1);
		if (str[has_env_var(str, diff) + 1] == '?')
			new = sub_status_var(str, has_env_var(str, diff), status);
		else
			new = sub_env_var(str, has_env_var(str, diff), env);
		if (!new)
			return (free(str), NULL);
		free(str);
		str = new;
		diff = ft_strlen(new) - next_diff;
	}
	return (str);
}
