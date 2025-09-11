/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:02:55 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/11 11:35:41 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	unsigned char	*new_ptr;
	size_t			copy_size;
	size_t			i;

	if ((new_size == 0) && ptr)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = (unsigned char *)malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (!ptr)
		return (new_ptr);
	copy_size = new_size;
	if (old_size < new_size)
		copy_size = old_size;
	i = 0;
	while (i < copy_size)
	{
		new_ptr[i] = ((unsigned char *)(ptr))[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
