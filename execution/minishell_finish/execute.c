/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 21:42:14 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/10 13:31:23 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count(char **param, int *c)
{
	while (param[*c])
		(*c)++;
}

int	get_index(char *s, char c)
{
	int	i;
	int	ret;

	i = 0;
	ret = -1;
	while (s[i])
	{
		if (s[i] == c)
			ret = i;
		i++;
	}
	return (ret);
}

void	arr_builtins(int i, t_cmd *cmd, t_main_args *main_args)
{
	t_f	builtin[8];

	builtin[0] = my_echo;
	builtin[1] = my_export;
	builtin[2] = my_env;
	builtin[3] = my_unset;
	builtin[4] = my_cd;
	builtin[5] = my_exit;
	builtin[6] = my_pwd;
	vars.last_exit_sat = builtin[i](cmd, main_args);
}

void	handle_builtins(t_cmd *cmd, int *built)
{
	int		i;
	char	*cmd1;

	i = 0;
	cmd->cmd && (i = get_index(cmd->cmd, '/'));
	cmd1 = &((cmd->cmd)[i + 1]);
	if (cmd->cmd && !ft_strncmp(cmd1, "echo", 5))
		*built = 0;
	else if (cmd->cmd && !ft_strncmp(cmd1, "export", 7))
		*built = 1;
	else if (cmd->cmd && !ft_strncmp(cmd1, "env", 4))
		*built = 2;
	else if (cmd->cmd && !ft_strncmp(cmd1, "unset", 6))
		*built = 3;
	else if (cmd->cmd && !ft_strncmp(cmd1, "cd", 3))
		*built = 4;
	else if (cmd->cmd && !ft_strncmp(cmd1, "exit", 5))
		*built = 5;
	else if (cmd->cmd && !ft_strncmp(cmd1, "pwd", 4))
		*built = 6;
	else
		*built = -1;
}

char	*my_get_env(char *var, t_main_args *main_args)
{
	int	i;

	i = 0;
	while (main_args->env[i])
	{
		if (ft_strncmp(main_args->env[i], var, ft_strlen(var) + 1) == '=')
			return (main_args->env[i] + ft_strlen(var) + 1);
		i++;
	}
	return (NULL);
}

char	*bring_path(t_main_args *main_args, char *cmd)
{
	char	*path_value;
	int		i;
	char	**splited_path;
	char	*joined_cmd;

	path_value = my_get_env("PATH", main_args);
	if (!path_value)
		print_error("there is no PATH variable");
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

int	here_doc(char *delimeter)
{
	char	*here_line;
	int		fd[2];

	pipe(fd);
	here_line = delimeter;
	delimeter = ft_strjoin(here_line, "\n");
	free(here_line);
	while (1)
	{
		write(1, ">", 1);
		here_line = get_next_line(0);
		if (!here_line || !ft_strncmp(delimeter, here_line,
				ft_strlen(delimeter)))
			break ;
		write(fd[1], here_line, ft_strlen(here_line));
		free(here_line);
	}
	close(fd[1]);
	return (fd[0]);
}

int	file_out(t_list *outfiles)
{
	t_list	*tmp;
	int		fdout;
	int		ret;
	t_io	*file_info;

	ret = -1;
	tmp = outfiles;
	while (tmp)
	{
		file_info = tmp->content;
		if (ft_strncmp(file_info->type, ">>", 2))
			fdout = open(file_info->file, O_RDWR | O_TRUNC | O_CREAT, 0644);
		else
			fdout = open(file_info->file, O_RDWR | O_APPEND | O_CREAT, 0644);
		close(ret);
		ret = fdout;
		tmp = tmp->next;
	}
	return (ret);
}

int	file_in(t_list *infiles)
{
	int		fdin;
	int		ret;
	int		failed;
	t_list	*tmp;
	t_io	*file_info;

	failed = 0;
	ret = -1;
	tmp = infiles;
	while (tmp)
	{
		file_info = tmp->content;
		if (ft_strncmp(file_info->type, "<<", 2))
			fdin = open(file_info->file, O_RDONLY);
		else
			fdin = here_doc(file_info->file);
		if (fdin == -1)
			failed = 1;
		close(ret);
		ret = fdin;
		tmp = tmp->next;
	}
	if (failed)
		return (-1);
	return (ret);
}

int	size_cmd(t_cmd *cmd)
{
	int		count;
	t_cmd	*tmp;

	count = 0;
	tmp = cmd;
	while (tmp)
	{
		tmp = tmp->next;
		count++;
	}
	return (count);
}
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

void	allocat_pipe(int i, int **f)
{
	int	j;

	j = 0;
	while (i > j)
	{
		f[j] = malloc(sizeof(int) * 2);
		j++;
	}
}

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
void	dup2_and_excute(t_fds fds, t_main_args *main_args, t_cmd *cmd)
{
	char	*cmd1;
	int		built;

	built = 0;
	if (dup2(fds.fdin, 0) == -1)
		perror("dup2in");
	close(fds.fdin);
	if (dup2(fds.fdout, 1) == -1)
		perror("dup2out");
	close(fds.fdout);
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
	{
		cmd1 = bring_path(main_args, cmd->cmd);
		if (!cmd1)
		{
			write(2, "command not found\n", 18);
			exit(127);
		}
		if (execve(cmd1, cmd->param, main_args->env) == -1)
			print_error("execve");
	}
	exit(0);
}

void	make_process(int **fdpipe, int i, t_main_args *main_args, t_cmd *tmp,
		t_cmd *cmd, t_pids **pids)
{
	t_fds	fds;

	fds.pid = fork();
	if (fds.pid < 0)
		print_error("fork");
	else if (fds.pid == 0)
	{
		if (tmp == cmd)
			first_child(fdpipe[i], &fds, tmp);
		else if (tmp->next)
			middle_child(fdpipe, &fds, tmp, i);
		else
			last_child(fdpipe[i - 1], &fds, tmp);
		dup2_and_excute(fds, main_args, tmp);
	}
	else
	{
		if (i > 0)
		{
			close(fdpipe[i - 1][0]);
			close(fdpipe[i - 1][1]);
		}
		ft_lstadd_back3(pids, ft_lstnew3(fds.pid));
	}
}

void	only_one(t_cmd *cmd, t_main_args *main_args, int *built)
{
	t_fds	fds;
	t_list	*infiles;
	t_list	*outfiles;

	infiles = cmd->fles->input;
	outfiles = cmd->fles->output;
	fds.fdin = 0;
	fds.fdout = 0;
	handle_builtins(cmd, built);
	if (*built == -1)
		return ;
	if (infiles)
	{
		fds.fdin = file_in(infiles);
		if (fds.fdin == -1)
		{
			write(2, "No such file or directory\n", 26);
			return ;
		}
		if (dup2(fds.fdin, 0) == -1)
			perror("fdin ");
		close(fds.fdin);
	}
	if (outfiles)
	{
		fds.fdout = file_out(outfiles);
		if (fds.fdout == -1)
		{
			write(2, "No such file or directory\n", 26);
			return ;
		}
		if (dup2(fds.fdout, 1) == -1)
			perror("fdout");
		close(fds.fdout);
	}
	arr_builtins(*built, cmd, main_args);
}

void	wait_process(t_pids *pids)
{
	t_pids	*t;

	while (pids)
	{
		t = pids->next;
		waitpid(pids->pid, &vars.last_exit_sat, 0);
		free(pids);
		pids = t;
	}
}

void	execute(t_cmd *cmd, t_main_args *main_args, int num_cmds)
{
	int		**fdpipe;
	t_cmd	*tmp;
	int		i;
	int		built;
	t_pids	*pids;
	int		tmpin;
	int		tmpout;

	tmpin = dup(0);
	tmpout = dup(1);
	built = 0;
	pids = NULL;
	fdpipe = malloc(sizeof(int *) * (num_cmds));
	allocat_pipe(num_cmds, fdpipe);
	tmp = cmd;
	i = 0;
	while (tmp)
	{
		main_args->ac = 0;
		while ((tmp->param)[main_args->ac])
			(main_args->ac)++;
		if (num_cmds == 1)
			only_one(tmp, main_args, &built);
		if (i < num_cmds - 1)
			pipe(fdpipe[i]);
		if (built == -1 || num_cmds > 1)
		{
			make_process(fdpipe, i, main_args, tmp, cmd, &pids);
			if (i < num_cmds - 1)
				i++;
		}
		tmp = tmp->next;
	}
	free_pipe(num_cmds, fdpipe);
	dup2(tmpin, 0);
	close(tmpin);
	dup2(tmpout, 1);
	close(tmpout);
	wait_process(pids);
}
