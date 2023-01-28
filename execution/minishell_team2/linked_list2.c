/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/20 17:34:49 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*ft_lstlast2(t_cmd *lst)
{
	t_cmd	*h;

	h = lst;
	if (h == NULL)
		return (h);
	while (h->next)
		h = h->next;
	return (h);
}

void	ft_lstadd_back2(t_cmd **lst, t_cmd *new)
{
	if (!*lst)
		*lst = new;
	else
		ft_lstlast2(*lst)->next = new;
}

void	free_arr(char **arr, void (*del)(void *))
{
	int	i;

	i = 0;
	while (arr[i])
	{
		del(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_lstclear2(t_cmd **lst, void (*del)(void *))
{
	t_cmd	*h;
	t_cmd	*d;

	if (lst && del)
	{
		if (*lst == NULL)
			return ;
		d = *lst;
		h = (*lst)->next;
		if (d->param)
			free_arr(d->param, del);
		free(*lst);
		*lst = h;
		if (h)
			ft_lstclear2(lst, (*del));
	}
}

t_cmd	*ft_lstnew2(char *cmd, char **param)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (new == NULL)
		return (NULL);
	new->cmd = cmd;
	new->param = param;
	new->next = NULL;
	return (new);
}
