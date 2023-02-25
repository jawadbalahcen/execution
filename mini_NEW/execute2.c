/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:15:17 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 02:24:44 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_output(t_fds *fds, t_io *info_file)
{
	if (!ft_strncmp(info_file->type, ">>", 2))
	{
		fds->fdout >= 0 && close(fds->fdout);
		fds->fdout = open(info_file->file, O_RDWR | O_APPEND | O_CREAT, 0644);
		if (fds->fdout == -1)
		{
			perror(info_file->file);
			exit(1);
		}
	}
	else if (!ft_strncmp(info_file->type, ">", 2))
	{
		fds->fdout >= 0 && close(fds->fdout);
		fds->fdout = open(info_file->file, O_RDWR | O_TRUNC | O_CREAT, 0644);
		if (fds->fdout == -1)
		{
			perror(info_file->file);
			exit(1);
		}
	}
}

void	set_input(t_fds *fds, t_io *info_file, t_norm_sake *norm)
{
	if (!ft_strncmp(info_file->type, "<<", 2))
	{
		fds->fdin >= 0 && close(fds->fdin);
		fds->fdin = (norm->hd_pipes)[norm->index_here][0];
		(norm->index_here) += 1;
	}
	else if (!ft_strncmp(info_file->type, "<", 2))
	{
		fds->fdin >= 0 && close(fds->fdin);
		fds->fdin = open(info_file->file, O_RDONLY);
		if (fds->fdin == -1)
		{
			perror(info_file->file);
			exit(1);
		}
	}
}

char	*bring_path(t_main_args *main_args, char *cmd)
{
	char	*path_value;
	int		i;
	char	**splited_path;
	char	*joined_cmd;

	path_value = my_get_env("PATH", main_args);
	if (!path_value)
	{
		write(2, cmd, ft_strlen(cmd));
		(write(2, ": No such file or directory\n", 29), exit(127));
	}
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

int	here_doc(char **delimeter, int fd[2])
{
	char	*here_line;
	char	*tmp;

	here_line = *delimeter;
	*delimeter = ft_strjoin(here_line, "\n");
	free(here_line);
	while (1)
	{
		here_line = readline("> ");
		tmp = here_line;
		here_line && (here_line = ft_strjoin(tmp, "\n"));
		free(tmp);
		if (!here_line || !ft_strncmp(*delimeter, here_line,
				ft_strlen(*delimeter)))
		{
			free(here_line);
			break ;
		}
		change_str(&here_line, '\"');
		write(fd[1], here_line, ft_strlen(here_line));
		free(here_line);
	}
	close(fd[1]);
	return (fd[0]);
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
