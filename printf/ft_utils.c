/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 04:54:19 by dchernik          #+#    #+#             */
/*   Updated: 2025/04/21 13:47:12 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*buf;

	buf = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		buf[i] = '\0';
		i++;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	tsize;
	size_t	size_max;

	size_max = (size_t)-1;
	ptr = NULL;
	if ((nmemb == 0) || (size == 0))
	{
		ptr = (void *)malloc(1);
		if (ptr == NULL)
			return (NULL);
		return (ptr);
	}
	if (nmemb > size_max / size)
		return (NULL);
	tsize = nmemb * size;
	ptr = (void *)malloc(tsize);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, tsize);
	return (ptr);
}

int	ft_putstr_fd(char *s, int fd)
{
	int	slen;

	slen = ft_strlen(s);
	if (write(fd, s, slen) == -1)
		return (-1);
	return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
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
