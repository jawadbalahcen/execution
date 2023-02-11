/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/27 00:41:07 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pids	*ft_lstlast3(t_pids *lst)
{
	t_pids	*h;

	h = lst;
	if (h == NULL)
		return (h);
	while (h->next)
		h = h->next;
	return (h);
}

void	ft_lstadd_back3(t_pids **lst, t_pids *new)
{
	if (!*lst)
		*lst = new;
	else
		ft_lstlast3(*lst)->next = new;
}

// void	ft_lstclear3(t_pids **lst, void (*del)(void*))
// {
// 	t_pids	*h;
// 	t_pids	*d;

// 	if (lst && del)
// 	{
// 		if (*lst == NULL)
// 			return ;
// 		d = *lst;
// 		h = (*lst)->next;
// 		if (d->pid != NULL)
// 			(*del)(d->pid);
// 		free(*lst);
// 		*lst = h;
// 		if (h)
// 			ft_lstclear3(lst, (*del));
// 	}
// }

t_pids	*ft_lstnew3(int pid)
{
	t_pids	*new;

	new = (t_pids *)malloc(sizeof(t_pids));
	if (new == NULL)
		return (NULL);
	new->pid = pid;
	new->next = NULL;
	return (new);
}
