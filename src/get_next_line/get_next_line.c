/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 21:32:15 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/30 18:28:14 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_line(ssize_t rd, char *str)
{
	int	i;

	i = 0;
	if (rd < BUFFER_SIZE && rd > 0)
		return (i);
	if (str)
	{
		while (str[i])
		{
			if (str[i] == '\n')
				return (i);
			i++;
		}
	}
	return (-1);
}

char	*ft_dupcat(char *dest, char *src)
{
	size_t	lendest;
	char	*tmp;

	tmp = NULL;
	lendest = 0;
	if (dest)
	{
		tmp = ft_strdup(dest);
		if (!tmp)
			return (NULL);
		lendest = ft_strlen(tmp);
		free(dest);
	}
	dest = malloc(sizeof(char) * (lendest + ft_strlen(src) + 1));
	dest[0] = '\0';
	if (!dest)
		return (NULL);
	if (tmp)
	{
		dest = ft_strcat(dest, tmp);
		free(tmp);
	}
	dest = ft_strcat(dest, src);
	return (dest);
}

char	*ft_dup_line(char *str)
{
	char	*line;
	size_t	len;
	size_t	i;

	len = 0;
	i = 0;
	while (str[len] != '\0' && str[len] != '\n')
		len++;
	if (str[len] == '\n')
		len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	while (i < len)
	{
		line[i] = str[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_remove_line(char *save)
{
	char	*new;
	size_t	skip;
	size_t	i;

	skip = 0;
	i = 0;
	while (save[skip] != '\n' && save[skip] != '\0')
		skip++;
	if (save[skip] == '\0')
		skip--;
	new = malloc(sizeof(char) * (ft_strlen(save) - skip));
	if (!new)
	{
		free(save);
		return (NULL);
	}
	while (save[++skip])
	{
		new[i] = save[skip];
		i++;
	}
	new[i] = '\0';
	free(save);
	return (new);
}

char	*get_next_line(int fd)
{
	static char	*save = NULL;
	static char	txt[BUFFER_SIZE + 1];
	char		*line;
	ssize_t		rd;

	rd = 0;
	while (ft_check_line(rd, save) == -1)
	{
		rd = read(fd, txt, BUFFER_SIZE);
		if (rd == 0 && save != NULL)
			break ;
		if (rd <= 0)
			return (ft_exit_error(&save));
		txt[rd] = '\0';
		save = ft_dupcat(save, txt);
		if (!save)
			return (ft_exit_error(&save));
	}
	line = ft_dup_line(save);
	save = ft_remove_line(save);
	if (!line || !save)
		return (ft_exit_error_duo(&save, &line));
	if (ft_strlen(save) == 0)
		ft_exit_error(&save);
	return (line);
}
