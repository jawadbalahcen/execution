/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_to_cmd_geters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/11 21:01:19 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_input_files(char *str, char *type, t_list **lst)
{
	t_list	*new;
	t_io	*io;

	io = malloc(sizeof(t_io));
	if (io == NULL)
		exit(1);
	io->type = type;
	io->file = str;
	new = ft_lstnew(io);
	if (new == NULL)
		exit(1);
	ft_lstadd_back(lst, new);
}

t_list	*get_input_files(int count, t_list *lst)
{
	t_list	*in;

	in = NULL;
	while (count > 0)
	{
		if (((t_range *)lst->content)->type == 0)
		{
			if (ft_strcmp(((t_range *)lst->content)->str, "<") == 0)
			{
				insert_input_files(((t_range *)lst->next->content)->str,
					((t_range *)lst->content)->str, &in);
				count--;
			}
			if (ft_strcmp(((t_range *)lst->content)->str, "<<") == 0)
			{
				insert_input_files(((t_range *)lst->next->content)->str,
					((t_range *)lst->content)->str, &in);
				count--;
			}
		}
		lst = lst->next;
	}
	return (in);
}

t_list	*get_output_files(int count, t_list *lst)
{
	t_list	*in;

	in = NULL;
	while (count > 0)
	{
		if (((t_range *)lst->content)->type == 0)
		{
			if (ft_strcmp(((t_range *)lst->content)->str, ">") == 0)
			{
				insert_input_files(((t_range *)lst->next->content)->str,
					((t_range *)lst->content)->str, &in);
				count--;
			}
			if (ft_strcmp(((t_range *)lst->content)->str, ">>") == 0)
			{
				insert_input_files(((t_range *)lst->next->content)->str,
					((t_range *)lst->content)->str, &in);
				count--;
			}
		}
		lst = lst->next;
	}
	return (in);
}

char	**get_params(int count, t_list *lst)
{
	char	**param;
	int		i;

	param = init_param(count);
	i = 0;
	while (i < count)
	{
		if (((t_range *)lst->content)->str != NULL)
		{
			if (((t_range *)lst->content)->type != 0)
			{
				param[i] = ((t_range *)lst->content)->str;
				i++;
			}
			else
				lst = lst->next;
		}
		lst = lst->next;
	}
	return (param);
}
