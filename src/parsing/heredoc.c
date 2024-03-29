/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:19:35 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:19:36 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	swap_termios(void)
{
	struct termios	termi_tmp;

	tcgetattr(0, &termi_tmp);
	termi_tmp.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSAFLUSH, &termi_tmp);
}

static char	*loop_return(char *str1, char *str2)
{
	char	*new;

	new = ft_strjoin(str1, str2);
	free(str2);
	if (!new)
		return (NULL);
	return (new);
}

static char	*gnl_heredoc(char *limiter, int loop)
{
	char			*line;
	char			*new;
	struct termios	termi_og;

	g_sig = 1;
	tcgetattr(0, &termi_og);
	swap_termios();
	init_signal(S_HEREDOC);
	line = get_next_line(0);
	init_signal(S_DEFAULT);
	tcsetattr(0, TCSANOW, &termi_og);
	if (!line && !g_sig)
		return (NULL);
	else if (!line && !loop)
		return (printf_error(ERR_HDOC, limiter), ft_strjoin(limiter, "\n"));
	else if (!line)
		return (loop_return("", gnl_heredoc(limiter, 1)));
	if (line[ft_strlen(line) - 1] != '\n')
	{
		new = loop_return(line, gnl_heredoc(limiter, 1));
		free(line);
		return (new);
	}
	return (line);
}

int	fill_heredoc(int fd, char *limiter)
{
	char	*line;

	limiter = strdup_quotes(limiter);
	write(1, "> ", 2);
	line = gnl_heredoc(limiter, 0);
	while (line && !(ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& ft_strlen(line) == ft_strlen(limiter) + 1))
	{
		ft_putstr_fd(line, fd);
		free(line);
		write(1, "> ", 2);
		line = gnl_heredoc(limiter, 0);
	}
	close(fd);
	free(limiter);
	if (!line)
		return (1);
	free(line);
	return (0);
}
