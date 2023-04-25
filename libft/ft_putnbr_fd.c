/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:39:10 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/08 17:50:07 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	new;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		new = -n;
	}
	else
		new = n;
	if (new < 10)
		ft_putchar_fd(new + 48, fd);
	else
	{
		ft_putnbr_fd(new / 10, fd);
		ft_putnbr_fd(new % 10, fd);
	}
}
