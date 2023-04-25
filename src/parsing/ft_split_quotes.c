/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:19:50 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:19:50 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count_strings(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		while (str[i] == c)
			i++;
		if (str[i] != '\0')
		{
			while (str[i] != c && str[i] != '\0')
			{
				if (str[i] == '"' || str[i] == '\'')
					i = ft_skip_quotes(str, str[i], i + 1);
				i++;
			}
			count++;
		}
		while (str[i] == c && str[i] != '\0')
			i++;
	}
	return (count);
}

static char	*ft_dup_string(int i, int j, char *dest, char *str)
{
	int	pos;

	pos = 0;
	while (j < i)
	{
		dest[pos] = str[j];
		j++;
		pos++;
	}
	dest[pos] = '\0';
	return (dest);
}

static void	ft_secure_malloc(char **dest, int pos, int i, int j)
{
	dest[pos] = malloc(sizeof(char) * (i - j + 1));
	if (!dest[pos])
	{
		while (pos-- > 0)
			free(dest[pos]);
		free(dest);
	}
}

static char	**split_quotes_2(char **dest, char *str, char c, int len[2])
{
	int	pos;

	pos = 0;
	while (pos < ft_count_strings(str, c))
	{
		while (str[len[0]] == c)
			len[0]++;
		len[1] = len[0];
		while (str[len[0]] != c && str[len[0]] != '\0')
		{
			if (str[len[0]] == '"' || str[len[0]] == '\'')
					len[0] = ft_skip_quotes(str, str[len[0]], len[0] + 1);
			len[0]++;
		}
		ft_secure_malloc(dest, pos, len[0], len[1]);
		if (!dest)
			return (NULL);
		ft_dup_string(len[0], len[1], dest[pos], str);
		pos++;
	}
	dest[pos] = 0;
	return (dest);
}

char	**ft_split_quotes(char *s, char c)
{
	int		len[2];
	char	**dest;

	len[0] = 0;
	len[1] = 0;
	dest = malloc(sizeof(char *) * (ft_count_strings(s, c) + 1));
	if (!dest)
		return (NULL);
	dest = split_quotes_2(dest, s, c, len);
	return (dest);
}
