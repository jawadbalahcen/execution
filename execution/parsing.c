/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/23 21:13:44 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_char(unsigned int c, char *s)
{
	c = 0;
	if (*s == '\f' || *s == '\r' || *s == '\v' || *s == '\n' || *s == '\t'
		|| *s == ' ')
	{
		*s = ' ';
	}
}

void	print_linked(t_list *lst)
{
	while (lst)
	{
		printf("%s			(is_next_space %i)\n", lst->content,
							lst->next_is_space);
		lst = lst->next;
	}
}

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
		if (check_is_symbol(h->content[0]) == 0)
			alloc_cmd(h->content, 0);
		else
			ft_lstadd_back2(&cmds, alloc_cmd(NULL, 1));
		if (h->next == NULL && check_is_symbol(h->content[0]) == 0)
			ft_lstadd_back2(&cmds, alloc_cmd(NULL, 1));
		h = h->next;
	}
	return (cmds);
}

void	print_linked2(t_cmd *lst)
{
	int	i;

	while (lst)
	{
		i = 0;
		printf("\ncmd %s:\n", lst->cmd);
		printf("param:\n	");
		while (lst->param[i])
		{
			printf("%s ", lst->param[i]);
			i++;
		}
		printf("\n");
		lst = lst->next;
	}
}


void	handle_line(char *line, t_main_args *main_args)
{
	char	**split;
	t_list	*lst;
	t_cmd	*cmd;
	// int		fd[2];
	// int		built;
	// int		pid;
	// char	*cmd1;
	ft_striteri(line, change_char);
	split = ft_split(line, ' ');
	if (split == NULL)
		exit(1);
	lst = split_by_special_char(split);
	group_symbols(&lst);
	replace_env(&lst);
	cmd = conv_to_cmd(&lst);
	execute(cmd, main_args);
	// print_linked2(cmd);
	ft_lstclear2(&cmd, free);
	ft_lstclear(&lst, free);
}
