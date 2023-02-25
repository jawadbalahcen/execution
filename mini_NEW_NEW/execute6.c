/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 02:02:06 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 02:10:47 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	and_execute(t_main_args *main_args, t_cmd *cmd)
{
	char	*cmd1;

	cmd1 = bring_path(main_args, cmd->cmd);
	if (!cmd1 || !(cmd->cmd)[0])
	{
		if (cmd->cmd[0] == '.' && ft_strlen(cmd->cmd) == 1)
			print_exit(".: filename argument required\n.: usage: "
				". filename [arguments]\n", 2);
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		if (is_directory(cmd->cmd))
			print_exit(": is a directory\n", 126);
		write(2, ": command not found\n", 21);
		exit(127);
	}
	if (execve(cmd1, cmd->param, main_args->env) == -1)
		print_error("execve");
}

void	dup2_and(t_fds fds, t_cmd *cmd)
{
	if (dup2(fds.fdin, 0) == -1)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		perror("dup2in");
	}
	close(fds.fdin);
	if (dup2(fds.fdout, 1) == -1)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		perror("dup2out");
	}
	close(fds.fdout);
}
