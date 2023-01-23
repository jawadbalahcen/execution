/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/18 18:15:15 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_value(t_list **lst)
{
	t_list	*h;
	char	*env_val;

	h = (*lst);
	while (h)
	{
		if (h->content[0] == '$' && h->content[1] != 0)
		{
			env_val = getenv(h->content + 1);
			free(h->content);
			if (env_val == NULL)
				h->content = NULL;
			else
				h->content = ft_substr(env_val, 0, ft_strlen(env_val));
		}
		h = h->next;
	}
}

void	join_nodes(t_list **h)
{
	t_list	*todel;
	char	*new_str;
	int		space;

	todel = (*h)->next;
	space = todel->next_is_space;
	(*h)->next = todel->next;
	new_str = ft_strjoin((*h)->content, todel->content);
	free((*h)->content);
	(*h)->content = new_str;
	(*h)->next_is_space = space;
	free(todel->content);
	free(todel);
}

void	join_spaces(t_list **lst)
{
	t_list	*h;
	char	c;
	char	cn;

	h = (*lst);
	while (h->next)
	{
		if (h->next_is_space == 0)
		{
			c = check_is_symbol(h->content[0]);
			cn = check_is_symbol(h->next->content[0]);
			if (!c && c != '$' && !cn && cn != '$')
			{
				join_nodes(&h);
			}
		}
		if (h->next == NULL)
			break ;
		h = h->next;
	}
}

void	replace_env(t_list **lst)
{
	t_list	*h;

	h = (*lst);
	while (h->next)
	{
		if (h->content[0] == '$' && h->next_is_space == 0)
		{
			if (check_is_symbol(h->next->content[0]) == 0)
				join_nodes(&h);
		}
		if (h->next == NULL)
			break ;
		h = h->next;
	}
	replace_value(lst);
	join_spaces(lst);
}
