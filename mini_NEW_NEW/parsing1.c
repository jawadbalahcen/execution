/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 21:08:33 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 14:48:28 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	qoutes_stop(char *line2, t_list *lst)
{
	ft_lstclear(&lst, free_range_arr);
	free(line2);
}

void	handle_line(char *line, t_main_args *args)
{
	char	*line2;
	t_cmd	*cmd;
	t_qoute	res;

	if (*line == 0)
		return ;
	line2 = ft_strtrim(line, " \f\v\n\r\t");
	if (line2 == NULL)
		exit(1);
	res = qoutes_handling(line);
	if (res.err == 1)
	{
		qoutes_stop(line2, res.lst);
		return ;
	}
	set_expandale(&res.lst);
	replace_env(&res.lst);
	join_lines(&res.lst);
	cmd = conv_to_cmd(&res.lst);
	if (cmd != NULL)
		execute(cmd, args, size_cmd(cmd));
	free(line2);
	ft_lstclear2(&cmd, free);
	ft_lstclear(&res.lst, free);
}
