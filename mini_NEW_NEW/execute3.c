/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:20:01 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/24 17:53:51 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_files(t_list *order_file, t_fds *fds, t_norm_sake *norm)
{
	t_io	*info_file;
	t_list	*tmp;

	tmp = order_file;
	fds->fdout = -2;
	fds->fdin = -2;
	while (tmp)
	{
		info_file = tmp->content;
		set_output(fds, info_file);
		set_input(fds, info_file, norm);
		tmp = tmp->next;
	}
}

void	first_child(int *fdpipe, t_fds *fds, t_cmd *cmd, t_norm_sake *norm)
{
	t_list	*order_file;

	order_file = cmd->fles->order;
	open_files(order_file, fds, norm);
	if (fds->fdin != -2)
	{
		if (fds->fdin == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdin = dup(0);
	if (fds->fdout != -2)
	{
		close(fdpipe[1]);
		if (fds->fdout == -1)
			print_error("No such file or directory");
	}
	else if (!cmd->next)
		fds->fdout = dup(1);
	else
		fds->fdout = fdpipe[1];
	close(fdpipe[0]);
}

void	middle_child(int **fdpipe, t_fds *fds, t_cmd *cmd, t_norm_sake *norm)
{
	t_list	*order_file;

	order_file = cmd->fles->order;
	open_files(order_file, fds, norm);
	if (fds->fdin != -2)
	{
		close(fdpipe[norm->i - 1][0]);
		if (fds->fdin == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdin = fdpipe[norm->i - 1][0];
	if (fds->fdout != -2)
	{
		close(fdpipe[norm->i][1]);
		if (fds->fdout == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdout = fdpipe[norm->i][1];
	close(fdpipe[norm->i][0]);
	close(fdpipe[norm->i - 1][1]);
}

void	last_child(int *fdpipe, t_fds *fds, t_cmd *cmd, t_norm_sake *norm)
{
	t_list	*order_file;

	order_file = cmd->fles->order;
	open_files(order_file, fds, norm);
	if (fds->fdin != -2)
	{
		close(fdpipe[0]);
		if (fds->fdin == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdin = fdpipe[0];
	if (fds->fdout != -2)
	{
		if (fds->fdout == -1)
			print_error("No such file or directory");
	}
	else
		fds->fdout = dup(1);
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
