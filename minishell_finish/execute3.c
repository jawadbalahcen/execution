/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:20:01 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/11 20:20:33 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *s)
{
	write(2, s, ft_strlen(s));
	write(2, "\n", 1);
	exit(1);
}

void	first_child(int *fdpipe, t_fds *fds, t_cmd *cmd)
{
	t_list	*infiles;
	t_list	*outfiles;

	infiles = cmd->fles->input;
	outfiles = cmd->fles->output;
	if (outfiles)
	{
		close(fdpipe[1]);
		fds->fdout = file_out(outfiles);
		if (fds->fdout == -1)
			print_error("No such file or directory");
	}
	else if (!cmd->next)
		fds->fdout = dup(1);
	else
		fds->fdout = fdpipe[1];
	if (infiles)
	{
		fds->fdin = file_in(infiles);
		if (fds->fdin == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdin = dup(0);
	close(fdpipe[0]);
}

void	middle_child(int **fdpipe, t_fds *fds, t_cmd *cmd, int i)
{
	t_list	*infiles;
	t_list	*outfiles;

	infiles = cmd->fles->input;
	outfiles = cmd->fles->output;
	if (outfiles)
	{
		close(fdpipe[i][1]);
		fds->fdout = file_out(outfiles);
		if (fds->fdout == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdout = fdpipe[i][1];
	if (infiles)
	{
		close(fdpipe[i - 1][0]);
		fds->fdin = file_in(infiles);
		if (fds->fdin == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdin = fdpipe[i - 1][0];
	close(fdpipe[i][0]);
	close(fdpipe[i - 1][1]);
}

void	last_child(int *fdpipe, t_fds *fds, t_cmd *cmd)
{
	t_list	*infiles;
	t_list	*outfiles;

	infiles = cmd->fles->input;
	outfiles = cmd->fles->output;
	if (outfiles)
	{
		fds->fdout = file_out(outfiles);
		if (fds->fdout == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdout = dup(1);
	if (infiles)
	{
		close(fdpipe[0]);
		fds->fdin = file_in(infiles);
		if (fds->fdin == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdin = fdpipe[0];
	close(fdpipe[1]);
}

void	allocat_pipe(int num_cmds, t_norm_sake *norm)
{
	int	j;

	norm->fdpipe = malloc(sizeof(int *) * (num_cmds));
	j = 0;
	while (num_cmds > j)
	{
		(norm->fdpipe)[j] = malloc(sizeof(int) * 2);
		j++;
	}
}
