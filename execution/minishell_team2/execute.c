/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 21:42:14 by jbalahce          #+#    #+#             */
/*   Updated: 2023/01/28 12:13:20 by jbalahce         ###   ########.fr       */
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
	if (cmd->cmd && !ft_strncmp(cmd->cmd, "echo", 5))
		my_echo(cmd, main_args);
	else if (cmd->cmd && !ft_strncmp(cmd->cmd, "export", 7))
		my_export(cmd, main_args);
	else if (cmd->cmd && !ft_strncmp(cmd->cmd, "env", 4))
		my_env(main_args);
	else if (cmd->cmd && !ft_strncmp(cmd->cmd, "unset", 6))
		my_unset(cmd, main_args);
	else if (cmd->cmd && !ft_strncmp(cmd->cmd, "cd", 3))
		my_cd(cmd, main_args);
	else if (cmd->cmd && !ft_strncmp(cmd->cmd, "exit", 5))
		my_exit();
	else if (cmd->cmd && !ft_strncmp(cmd->cmd, "pwd", 4))
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
	if (!cmd || cmd[0] == '/' || cmd[0] == '.')
		return (NULL);
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
	return (NULL);
}

void	execute(t_cmd *cmd, t_main_args *main_args)
{
	int		tmpin;
	int		tmpout;
	int		fdin;
	int		ret;
	int		built;
	t_cmd	*tmp;
	int		fdout;
	char	*cmd1;
	char	*infile;
	char	*outfile;
	int		fdpipe[2];
	t_pids	*pids;
	t_pids	*t;

	pids = NULL;
	infile = NULL;
	outfile = NULL;
	tmpin = dup(0);
	tmpout = dup(1);
	if (infile)
		fdin = open(infile, O_RDONLY);
	else
		fdin = dup(tmpin);
	tmp = cmd;
	while (tmp)
	{
		main_args->ac = 0;
		if (dup2(fdin, 0) == -1)
			perror("dup2");
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
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		if (dup2(fdout, 1) == -1)
			perror("dup2");
		close(fdout);
		built = 0;
		// if (!tmp->cmd)
		// {
		// 	write(2, "command not found\n", 18);
		// 	tmp = tmp->next;
		// 	continue ;
		// }
		while ((tmp->param)[main_args->ac])
			(main_args->ac)++;
		handle_builtins(tmp, main_args, &built);
		if (built)
		{
			ret = fork();
			if (ret < 0)
				perror("fork");
			if (ret == 0)
			{
				execve(tmp->cmd, tmp->param, main_args->env);
				cmd1 = bring_path(main_args, tmp->cmd);
				if (!cmd1)
				{
					write(2, "command not found\n", 18);
					exit(127);
				}
				close(fdout);
				close(fdin);
				if (execve(cmd1, tmp->param, main_args->env) == -1)
					perror("execve");
			}
			else
				ft_lstadd_back3(&pids, ft_lstnew3(ret));
		}
		tmp = tmp->next;
	}
	dup2(tmpin, 0);
	dup2(tmpout, 1);
	close(tmpin);
	close(tmpout);
	t = pids;
	while (t)
	{
		waitpid(t->pid, &vars.last_exit_sat, 0);
		t = t->next;
	}
	// if (!background)
	// 	waitpid(ret, NULL);
}

/*
void	execute(t_cmd *cmd, t_main_args *main_args)
{
	int	id;

	(void)main_args;
	(void)cmd;
	id = fork();
	if (id == 0)
	{
		printf("%s\n", cmd->cmd);
		execvp(cmd->cmd, cmd->param);
	}
	else if (id != 0)
	{
		wait(NULL);
	}
}
*/
