/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:51:43 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/11 13:10:07 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*dest;

	i = 0;
	dest = malloc(sizeof(char) * len + 1);
	if (!dest || !s)
		return (NULL);
	if (start < ft_strlen(s))
	{
		while (s[start] && i < len)
		{
			dest[i] = s[start];
			i++;
			start++;
		}
	}
	dest[i] = '\0';
	return (dest);
}
