/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 21:08:33 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/11 21:16:03 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_io(t_list *lst)
{
	while (lst)
	{
		printf("	%s %s\n", ((t_io *)lst->content)->type,
			((t_io *)lst->content)->file);
		lst = lst->next;
	}
}

void	print_cmds(t_cmd *lst)
{
	int	i;

	printf("+++CMDS++++\n");
	while (lst)
	{
		printf("cmd = %s\n", lst->cmd);
		i = 0;
		while (lst->param[i])
		{
			printf("	%s\n", lst->param[i]);
			i++;
		}
		printf("infiles \n");
		print_io(lst->fles->input);
		printf("outfiles \n");
		print_io(lst->fles->output);
		printf("token '%s'\n", lst->token);
		lst = lst->next;
	}
}

void	handle_line(char *line, t_main_args *args)
{
	char	*line2;
	t_list	*lst;
	t_cmd	*cmd;
	int		num_cmds;

	line2 = ft_strtrim(line, " \f\v\n\r\t");
	if (line2 == NULL)
		exit(1);
	lst = qoutes_handling(line2);
	replace_env(&lst);
	join_lines(&lst);
	cmd = conv_to_cmd(&lst);
	num_cmds = size_cmd(cmd);
	if (cmd != NULL)
		execute(cmd, args, num_cmds);
	ft_lstclear2(&cmd, free);
	ft_lstclear(&lst, free);
	free(line2);
}
