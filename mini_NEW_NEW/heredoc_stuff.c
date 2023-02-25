/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_stuff.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 00:40:55 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 15:23:47 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_here_doc(t_io *file, int pipe[2], int *tmpin, int *i)
{
	if (!ft_strncmp(file->type, "<<", 2))
	{
		*tmpin > 0 && close(*tmpin);
		*tmpin = here_doc(&file->file, pipe);
		(*i)++;
	}
}

void	open_heredocs(t_cmd *cmd, t_norm_sake *norm, struct termios term)
{
	t_list	*tmp_files;
	t_cmd	*tmp_cmd;
	t_io	*file;
	int		tmpin;
	int		i;

	i = 0;
	(void)term;
	disable_sigquit(&term);
	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		tmpin = -2;
		tmp_files = tmp_cmd->fles->order;
		while (tmp_files)
		{
			file = tmp_files->content;
			if (i < norm->num_heredoc)
				is_here_doc(file, (norm->hd_pipes)[i], &tmpin, &i);
			tmp_files = tmp_files->next;
		}
		tmp_cmd = tmp_cmd->next;
	}
	tcsetattr(0, TCSANOW, &term);
	exit(0);
}

void	allocat_hd_pipe(int num_heredoc, t_norm_sake *norm)
{
	int	j;

	norm->hd_pipes = malloc(sizeof(int *) * (num_heredoc));
	j = 0;
	while (num_heredoc > j)
	{
		(norm->hd_pipes)[j] = malloc(sizeof(int) * 2);
		j++;
	}
}

void	is_here_doc2(t_io *file, int **pipes, int *tmp_i, int *i)
{
	if (!ft_strncmp(file->type, "<<", 2))
	{
		if (*tmp_i != -1 && !close(pipes[*tmp_i][0]))
			pipes[*tmp_i][0] = -1;
		if (*tmp_i != -1 && !close(pipes[*tmp_i][1]))
			pipes[*tmp_i][1] = -1;
		*tmp_i = *i;
		(*i)++;
	}
}

void	close_hd_pipes(t_cmd *cmd, t_norm_sake *norm)
{
	t_io	*file;
	t_list	*tmp_files;
	t_cmd	*tmp_cmd;
	int		i;
	int		tmp_i;

	i = 0;
	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		tmp_i = -1;
		tmp_files = tmp_cmd->fles->order;
		while (tmp_files)
		{
			file = tmp_files->content;
			is_here_doc2(file, norm->hd_pipes, &tmp_i, &i);
			tmp_files = tmp_files->next;
		}
		if (tmp_i != -1 && !close((norm->hd_pipes)[tmp_i][1]))
			(norm->hd_pipes)[tmp_i][1] = -1;
		tmp_cmd = tmp_cmd->next;
	}
}
