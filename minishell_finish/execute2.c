/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:15:17 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/11 20:18:21 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
