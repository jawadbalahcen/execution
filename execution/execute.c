/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 21:42:14 by jbalahce          #+#    #+#             */
/*   Updated: 2023/01/23 21:14:49 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	count(char **param, int *c)
{
	while (param[*c])
		(*c)++;
}

void	handle_builtins(t_cmd *cmd, t_main_args *main_args, int *built)
{
	if (!ft_strncmp(cmd->cmd, "echo", 4))
		my_echo(cmd, main_args);
	else if (!ft_strncmp(cmd->cmd, "export", 6))
		my_export(cmd, main_args);
	else if (!ft_strncmp(cmd->cmd, "env", 3))
		my_env(main_args);
	else if (!ft_strncmp(cmd->cmd, "unset", 4))
		my_unset(cmd, main_args);
	else if (!ft_strncmp(cmd->cmd, "cd", 2))
		my_cd(cmd, main_args);
	else if (!ft_strncmp(cmd->cmd, "exit", 4))
		my_exit();
	else if (!ft_strncmp(cmd->cmd, "pwd", 3))
		my_pwd(main_args);
	else
		*built = 1;
}

char	*my_get_env(char *var, t_main_args *main_args)
{
	int	i;

	i = 0;
	while (main_args->env[i] && ft_strncmp(main_args->env[i], var,
			ft_strlen(var)))
		i++;
	if (main_args->env[i])
		return (main_args->env[i] + ft_strlen(var) + 1);
	return (main_args->env[i]);
}

char	*bring_path(t_main_args *main_args, char *cmd)
{
	char	*path_value;
	int		i;
	char	**splited_path;
	char	*joined_cmd;

	path_value = my_get_env("PATH", main_args);
	if (!path_value)
		perror("there is no PATH variable");
	splited_path = ft_split(path_value, ':');
	cmd = ft_strjoin("/", cmd);
	i = 0;
	while (splited_path[i])
	{
		joined_cmd = ft_strjoin(splited_path[i], cmd);
		if (!access(joined_cmd, X_OK))
			return (joined_cmd);
		free(joined_cmd);
		i++;
	}
	// perror(cmd);
	return (NULL);
}

void	execute(t_cmd *cmd, t_main_args *main_args)
{
	int tmpin;
	int tmpout;
	int fdin;
	int ret;
	int built;
	t_cmd *tmp;
	int fdout;
	char *cmd1;

	char *infile = NULL;
	char *outfile = NULL;
	tmpin = dup(0);
	tmpout = dup(1);
	if (infile)
		fdin = open(infile, O_RDONLY);
	else
		fdin = dup(tmpin);
	tmp = cmd;
	while (tmp)
	{
		dup2(fdin, 0);
		close(fdin);
		if (!tmp->next)
		{
			if (outfile)
				fdout = open(outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
			else
				fdout = dup(tmpout);
		}
		else
		{
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		dup2(fdout, 1);
		close(fdout);
		ret = fork();
		built = 0;
		if (ret == 0)
		{
			cmd1 = bring_path(main_args, tmp->cmd);
			// write(1, "\n", 1);
			while ((cmd->param)[main_args->ac])
				(main_args->ac)++;
			handle_builtins(tmp, main_args, &built);
			if (built)
			{
				if (execve(cmd1, tmp->param, main_args->env) == -1)
				{
					perror("execve1");
					exit(errno);
				}
			}
		}
		tmp = tmp->next;
	}
	dup2(tmpin, 0);
	dup2(tmpout, 1);
	close(tmpin);
	close(tmpout);
	// waitpid(ret, NULL, 0);
	// if (!background)
	// 	waitpid(ret, NULL);
}