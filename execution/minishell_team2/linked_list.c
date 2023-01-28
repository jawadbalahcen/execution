/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/21 17:57:27 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*h;

	h = lst;
	if (h == NULL)
		return (h);
	while (h->next)
		h = h->next;
	return (h);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!*lst)
		*lst = new;
	else
		ft_lstlast(*lst)->next = new;
}

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*h;
	t_list	*d;

	if (lst && del)
	{
		if (*lst == NULL)
			return ;
		d = *lst;
		h = (*lst)->next;
		if (d->content != NULL)
			(*del)(d->content);
		free(*lst);
		*lst = h;
		if (h)
			ft_lstclear(lst, (*del));
	}
}

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
