/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:22:32 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 15:29:36 by jbalahce         ###   ########.fr       */
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

void	print_exit(char *s, int status)
{
	write(2, s, ft_strlen(s));
	exit(status);
}

int	is_directory(char *cmd)
{
	int	i;
	int	yes;

	i = 0;
	yes = 0;
	while (cmd[i])
	{
		if (cmd[i] != '/' && cmd[i] != '.')
			return (0);
		if (cmd[i] == '/')
			yes = 1;
		i++;
	}
	if (yes)
		return (1);
	return (0);
}

void	dup2_and_excute(t_fds fds, t_main_args *main_args, t_cmd *cmd)
{
	int	built;

	built = 0;
	dup2_and(fds, cmd);
	if (!access(cmd->cmd, F_OK))
	{
		handle_builtins(cmd, &built);
		if (built == -1)
			execve(cmd->cmd, cmd->param, main_args->env);
		else
			arr_builtins(built, cmd, main_args, fds.pid);
	}
	else
	{
		built = 0;
		handle_builtins(cmd, &built);
		if (built != -1)
			arr_builtins(built, cmd, main_args, fds.pid);
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
			first_child((norm->fdpipe)[norm->i], &fds, tmp, norm);
		else if (tmp->next)
			middle_child((norm->fdpipe), &fds, tmp, norm);
		else
			last_child((norm->fdpipe)[norm->i - 1], &fds, tmp, norm);
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
