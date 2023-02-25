/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_stuff2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 01:33:55 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 01:57:53 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_heredocs(t_cmd *cmd)
{
	t_io	*file;
	t_list	*tmp_files;
	t_cmd	*tmp_cmd;
	int		num_heredoc;

	num_heredoc = 0;
	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		tmp_files = tmp_cmd->fles->order;
		while (tmp_files)
		{
			file = tmp_files->content;
			if (!ft_strncmp(file->type, "<<", 2))
				num_heredoc++;
			tmp_files = tmp_files->next;
		}
		tmp_cmd = tmp_cmd->next;
	}
	return (num_heredoc);
}

void	creat_pipes_heredoc(t_cmd *cmd, t_norm_sake *norm)
{
	int	i;

	i = 0;
	norm->num_heredoc = count_heredocs(cmd);
	allocat_hd_pipe(norm->num_heredoc, norm);
	while (norm->num_heredoc > i)
	{
		pipe((norm->hd_pipes)[i]);
		i++;
	}
}

void	loop_cmds(t_main_args *main_args, t_norm_sake *norm, int num_cmds,
		t_cmd *cmd)
{
	while (norm->tmp)
	{
		norm->index_here = norm->num_heredoc - count_heredocs(norm->tmp);
		main_args->ac = count(norm->tmp->param, 0);
		if (num_cmds == 1)
			g_vars.last_exit_sat = only_one(norm->tmp, main_args, &norm->built,
					norm);
		if (norm->i < num_cmds - 1)
			pipe(norm->fdpipe[norm->i]);
		if (norm->built == -1 || num_cmds > 1)
		{
			make_process(main_args, norm->tmp, cmd, norm);
			if (norm->i < num_cmds - 1)
				(norm->i)++;
		}
		norm->tmp = norm->tmp->next;
	}
}
