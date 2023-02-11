/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 21:42:14 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/11 21:04:39 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_process(t_pids *pids)
{
	t_pids	*t;

	while (pids)
	{
		t = pids->next;
		waitpid(pids->pid, &g_vars.last_exit_sat, 0);
		free(pids);
		pids = t;
	}
}

void	begin(int num_cmds, t_norm_sake *norm, t_cmd *cmd)
{
	norm->tmpin = dup(0);
	norm->tmpout = dup(1);
	norm->built = 0;
	norm->pids = NULL;
	allocat_pipe(num_cmds, norm);
	norm->tmp = cmd;
	norm->i = 0;
}

void	finish(int num_cmds, t_norm_sake norm)
{
	free_pipe(num_cmds, norm.fdpipe);
	dup2(norm.tmpin, 0);
	close(norm.tmpin);
	dup2(norm.tmpout, 1);
	close(norm.tmpout);
	wait_process(norm.pids);
}

void	execute(t_cmd *cmd, t_main_args *main_args, int num_cmds)
{
	t_norm_sake	norm;

	begin(num_cmds, &norm, cmd);
	while (norm.tmp)
	{
		main_args->ac = count(norm.tmp->param, 0);
		if (num_cmds == 1)
			only_one(norm.tmp, main_args, &norm.built);
		if (norm.i < num_cmds - 1)
			pipe(norm.fdpipe[norm.i]);
		if (norm.built == -1 || num_cmds > 1)
		{
			make_process(main_args, norm.tmp, cmd, &norm);
			if (norm.i < num_cmds - 1)
				(norm.i)++;
		}
		norm.tmp = norm.tmp->next;
	}
	finish(num_cmds, norm);
}
