/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 04:54:19 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/10 16:08:19 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_putstring_fd(char *s, int fd)
{
	size_t	slen;

	slen = ft_strlen(s);
	if (write(fd, s, slen) == -1)
		return (-1);
	return (0);
}

char	*ft_itoa_pos(unsigned int n)
{
	int		i;
	char	*buf;

	if (n == 0)
	{
		buf = (char *)malloc(2 * sizeof(char));
		if (buf == NULL)
			return (NULL);
		buf[0] = '0';
		buf[1] = '\0';
		return (buf);
	}
	i = 0;
	buf = (char *)malloc((count_digits(n) + 1) * sizeof(char));
	if (buf == NULL)
		return (NULL);
	while (n != 0)
	{
		buf[i] = (n % 10) + '0';
		n /= 10;
		i++;
	}
	buf[i] = '\0';
	return (reverse_str(buf));
}
