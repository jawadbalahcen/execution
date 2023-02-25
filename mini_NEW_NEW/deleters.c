/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleters.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/21 14:11:49 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_io(void *ios)
{	
	free(((t_io *)ios)->file);
	free(((t_io *)ios)->type);
	free(ios);
}

void	del_range(void *range)
{
	if ((t_range *)range == NULL)
		return ;
	free(((t_range *)range)->str);
	free((t_range *)range);
}

void	free_arr(t_cmd **arr, void (*del)(void *))
{
	int	i;

	i = 0;
	while ((*arr)->param[i])
	{
		del((*arr)->param[i]);
		i++;
	}
	del((*arr)->param);
	ft_lstclear(&((*arr)->fles->order), free_io);
	if ((*arr)->token != NULL)
		free((*arr)->token);
	free((*arr)->fles);
}

void	free_range_arr(void *rang)
{
	t_range	*range;

	range = (t_range *)rang;
	if (range->str != NULL)
		free(range->str);
	free(range);
}
