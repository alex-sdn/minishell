/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:15:26 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/11 18:10:55 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_checkset(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_alloc(int trim1, int trim2, int len)
{
	char	*dest;

	if (trim1 == len)
		dest = malloc(sizeof(char) * 1);
	else
		dest = malloc(sizeof(char) * (trim2 - trim1 + 2));
	if (!dest)
		return (NULL);
	return (dest);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		trim1;
	int		trim2;
	char	*dest;

	i = 0;
	trim1 = 0;
	trim2 = (int)ft_strlen(s1) - 1;
	while (ft_checkset(s1[trim1], set) == 1)
		trim1++;
	while (ft_checkset(s1[trim2], set) == 1)
		trim2--;
	dest = ft_alloc(trim1, trim2, (int)ft_strlen(s1));
	if (!dest)
		return (NULL);
	while (trim1 <= trim2)
	{
		dest[i] = s1[trim1];
		i++;
		trim1++;
	}
	dest[i] = '\0';
	return (dest);
}
