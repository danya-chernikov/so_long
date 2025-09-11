/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_remove_if.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:36:22 by dchernik          #+#    #+#             */
/*   Updated: 2025/09/10 16:37:28 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void	delete_node(t_list **begin_list, t_list *ptr,
				t_list *priv, t_list *tmp);

/* After this function's execution *begin_list must not change */
void	ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
	t_list	*ptr;
	t_list	*priv;
	t_list	*tmp;

	if (!begin_list || !(*begin_list) || !data_ref || !cmp)
		return ;
	ptr = *begin_list;
	tmp = NULL;
	priv = NULL;
	while (ptr != NULL)
	{
		if (!cmp(data_ref, (void *)ptr->content))
		{
			delete_node(begin_list, ptr, priv, tmp);
			continue ;
		}
		else
		{
			priv = ptr;
			ptr = ptr->next;
		}
	}
}

static void	delete_node(t_list **begin_list, t_list *ptr,
	t_list *priv, t_list *tmp)
{
	if (priv == NULL)
	{
		*begin_list = ptr->next;
		free(ptr->content);
		free(ptr);
		ptr = *begin_list;
		priv = ptr;
	}
	else
	{
		tmp = ptr->next;
		priv->next = tmp;
		free(ptr->content);
		free(ptr);
		ptr = tmp;
	}
}
