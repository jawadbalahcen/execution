/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 21:42:14 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 15:37:15 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_not_file(int i, char *cmd, int *built)
{
	if (i != -1 && access(cmd, X_OK))
	{
		*built = -1;
		return (1);
	}
	return (0);
}

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

void	finish(int num_cmds, t_norm_sake norm, int i)
{
	int	j;

	j = 0;
	while (j < norm.num_heredoc)
	{
		(norm.hd_pipes)[j][0] != -1 && close((norm.hd_pipes)[j][0]);
		(norm.hd_pipes)[j][1] != -1 && close((norm.hd_pipes)[j][1]);
		j++;
	}
	free_pipe(norm.num_heredoc, norm.hd_pipes);
	if (i)
	{
		free_pipe(num_cmds, norm.fdpipe);
		dup2(norm.tmpin, 0);
		close(norm.tmpin);
		dup2(norm.tmpout, 1);
		close(norm.tmpout);
		wait_process(norm.pids);
	}
}

void	execute(t_cmd *cmd, t_main_args *main_args, int num_cmds)
{
	t_norm_sake		norm;
	int				pid;
	int				status;
	struct termios	old;

	tcgetattr(0, &old);
	creat_pipes_heredoc(cmd, &norm);
	pid = fork();
	g_vars.her_doc = pid;
	if (pid == 0)
		open_heredocs(cmd, &norm, old);
	else
	{
		waitpid(pid, &status, 0);
		tcsetattr(0, TCSANOW, &old);
		close_hd_pipes(cmd, &norm);
		if (WEXITSTATUS(status) == 0)
		{
			begin(num_cmds, &norm, cmd);
			loop_cmds(main_args, &norm, num_cmds, cmd);
			finish(num_cmds, norm, 1);
		}
		else
			finish(num_cmds, norm, 0);
	}
}
