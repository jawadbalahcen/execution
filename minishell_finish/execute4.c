/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:22:32 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/11 20:22:54 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipe(int i, int **f)
{
	int	j;

	j = 0;
	while (i > j)
	{
		free(f[j]);
		j++;
	}
	free(f);
}

void	dup2_and(t_fds fds)
{
	if (dup2(fds.fdin, 0) == -1)
		perror("dup2in");
	close(fds.fdin);
	if (dup2(fds.fdout, 1) == -1)
		perror("dup2out");
	close(fds.fdout);
}

void	and_execute(t_main_args *main_args, t_cmd *cmd)
{
	char	*cmd1;

	cmd1 = bring_path(main_args, cmd->cmd);
	if (!cmd1)
	{
		write(2, "command not found\n", 18);
		exit(127);
	}
	if (execve(cmd1, cmd->param, main_args->env) == -1)
		print_error("execve");
}

void	dup2_and_excute(t_fds fds, t_main_args *main_args, t_cmd *cmd)
{
	int	built;

	built = 0;
	dup2_and(fds);
	if (!access(cmd->cmd, F_OK))
	{
		handle_builtins(cmd, &built);
		if (built == -1)
			execve(cmd->cmd, cmd->param, main_args->env);
		else
			arr_builtins(built, cmd, main_args);
	}
	else
	{
		built = 0;
		handle_builtins(cmd, &built);
		if (built != -1)
			arr_builtins(built, cmd, main_args);
	}
	if (built == -1 && cmd->cmd)
		and_execute(main_args, cmd);
	exit(0);
}

void	make_process(t_main_args *main_args, t_cmd *tmp, t_cmd *cmd,
		t_norm_sake *norm)
{
	t_fds	fds;

	fds.pid = fork();
	if (fds.pid < 0)
		print_error("fork");
	else if (fds.pid == 0)
	{
		if (tmp == cmd)
			first_child((norm->fdpipe)[norm->i], &fds, tmp);
		else if (tmp->next)
			middle_child((norm->fdpipe), &fds, tmp, norm->i);
		else
			last_child((norm->fdpipe)[norm->i - 1], &fds, tmp);
		dup2_and_excute(fds, main_args, tmp);
	}
	else
	{
		if (norm->i > 0)
		{
			close(norm->fdpipe[norm->i - 1][0]);
			close(norm->fdpipe[norm->i - 1][1]);
		}
		ft_lstadd_back3(&(norm->pids), ft_lstnew3(fds.pid));
	}
}
