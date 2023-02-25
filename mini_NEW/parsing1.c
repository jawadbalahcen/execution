/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 21:08:33 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/20 18:23:05 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_line(char *line, t_main_args *args)
{
	char	*line2;
	t_list	*lst;
	t_cmd	*cmd;
	int		num_cmds;

	if (*line == 0)
		return ;
	line2 = ft_strtrim(line, " \f\v\n\r\t");
	if (line2 == NULL)
		exit(1);
	lst = qoutes_handling(line);
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
