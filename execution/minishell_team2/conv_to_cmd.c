/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_to_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/24 20:30:21 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*alloc_cmd(char *str, int alloc)
{
	static char	*p;
	char		**split;
	t_cmd		*r;

	if (alloc == 0)
	{
		p = ft_strjoin(p, " ");
		p = ft_strjoin(p, str);
		return (NULL);
	}
	else
	{
		split = ft_split(p, ' ');
		free(p);
		p = NULL;
		r = ft_lstnew2(split[0], split);
		if (r == NULL)
			exit(1);
		return (r);
	}
}

t_cmd	*conv_to_cmd(t_list **lst)
{
	t_cmd	*cmds;
	t_list	*h;

	cmds = NULL;
	h = *lst;
	while (h)
	{
		if (((t_range *)h->content)->type != 0)
			alloc_cmd(((t_range *)h->content)->str, 0);
		else
			ft_lstadd_back2(&cmds, alloc_cmd(NULL, 1));
		if (h->next == NULL && ((t_range *)h->content)->type != 0)
			ft_lstadd_back2(&cmds, alloc_cmd(NULL, 1));
		h = h->next;
	}
	return (cmds);
}
