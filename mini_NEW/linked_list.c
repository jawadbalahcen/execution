/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/21 00:45:54 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*tmp;
	int		count;

	count = 0;
	tmp = lst;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

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

void	ft_lstclear(t_list **lst, void (*del)(void *))
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
