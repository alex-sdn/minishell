/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:03:07 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/11 13:11:06 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	len1;
	size_t	len2;
	char	*dest;

	i = -1;
	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	dest = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!dest)
		return (NULL);
	while (s1[++i])
		dest[i] = s1[i];
	while (s2[i - len1])
	{
		dest[i] = s2[i - len1];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
