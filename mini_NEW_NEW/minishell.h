/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:57:56 by monabid           #+#    #+#             */
/*   Updated: 2023/02/25 20:45:23 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <termios.h>
# include <unistd.h>
# define CD_ER "cd: error retrieving current directory: "
# define ROR "getcwd: cannot access parent directories: "

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_fds
{
	int				fdin;
	int				fdout;
	int				pid;
}					t_fds;
typedef struct s_pids
{
	int				pid;
	struct s_pids	*next;
}					t_pids;

typedef struct s_qoute
{
	t_list			*lst;
	int				err;
}					t_qoute;

typedef struct s_range
{
	char			*str;
	int				type;
	int				next_is_space;
	int				expendable;
}					t_range;

typedef struct s_io
{
	char			*file;
	char			*type;
}					t_io;

typedef struct s_in_out
{
	t_list			*errfile;
	t_list			*order;
}					t_in_out;

typedef struct s_cmd
{
	char			*cmd;
	char			**param;
	t_in_out		*fles;
	char			*token;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_expend
{
	char			*start;
	t_list			*match;
	char			*dir;
}					t_expend;

typedef struct s_main_args
{
	int				ac;
	char			**av;
	char			**env;
	t_list			*env_lst;
}					t_main_args;

typedef struct s_g_vars
{
	t_main_args		*args;
	int				last_exit_sat;
	int				line_handled;
	pid_t			her_doc;
}					t_g_vars;

typedef struct s_help
{
	int				params;
	int				infiles;
	int				outfiles;
	int				nodes;
}					t_help;

typedef struct s_norm_sake
{
	int				built;
	int				tmpin;
	int				tmpout;
	int				i;
	int				index_here;
	t_pids			*pids;
	int				**fdpipe;
	t_cmd			*tmp;
	int				**hd_pipes;
	int				num_heredoc;
}					t_norm_sake;

t_g_vars			g_vars;

typedef int			(*t_f)(t_cmd *cmd, t_main_args *main_args);

//heredoc_stuff
void				disable_sigquit(struct termios *term);
void				open_heredocs(t_cmd *cmd, t_norm_sake *norm,
						struct termios term);
void				allocat_hd_pipe(int num_heredoc, t_norm_sake *norm);
void				close_hd_pipes(t_cmd *cmd, t_norm_sake *norm);

//heredoc_stuff2
int					count_heredocs(t_cmd *cmd);
void				creat_pipes_heredoc(t_cmd *cmd, t_norm_sake *norm);
void				loop_cmds(t_main_args *main_args, t_norm_sake *norm,
						int num_cmds, t_cmd *cmd);

// strings
size_t				ft_strlen(const char *s);
char				*ft_strjoin(char const *s1, char const *s2);
void				*ft_calloc(size_t count, size_t size);
char				*ft_strchr(const char *s, int c);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				**ft_split(char const *s, char c);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strncmp1(const char *s1, const char *s2, size_t n);
char				*get_next_line(int fd);
//string4
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_strdup(const char *s1);
int					ft_strcmp(const char *s1, const char *s2);
//string5

int					ft_isdigit(int c);
int					ft_isalpha(int c);
int					ft_isalnum(int c);
char				*ft_itoa(int n);
//string6
int					ft_toupset(int c);
char				*get_line(void);
//linked list
t_list				*ft_lstnew(void *content);
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstadd_back(t_list **lst, t_list *new);
t_list				*ft_lstlast(t_list *lst);
int					ft_lstsize(t_list *lst);
//step 1
char				check_is_symbol(char s);
int					is_space(char s);
t_qoute				qoutes_handling(char *line);
char				*alloc_str2(int i, int j, char *line);
void				insert(t_list **lst, char *str, int type, char space);
void				qoute_err(int *i, t_qoute *res);
//step 2
void				replace_env(t_list **lst);
char				*group_strs(char *start, char *var_vale, char *end);
int					replace_as_alpha(char **str, int i, int type);
int					replace_as_digit(char **str, int i, int type);
void				change_str(char **str, int type);
void				del_dollar(t_list **lst);
void				set_expandale(t_list **lst);
//link2
t_cmd				*ft_lstlast2(t_cmd *lst);
void				ft_lstadd_back2(t_cmd **lst, t_cmd *new);
void				ft_lstclear2(t_cmd **lst, void (*del)(void *));
t_cmd				*ft_lstnew2(char *cmd, char **param);
void				free_arr(t_cmd **arr, void (*del)(void *));
void				insert_input_files(char *str, char *type, t_list **lst);

//builtin_commands
int					my_cd(t_cmd *cmd, t_main_args *main_args);
int					my_pwd(t_cmd *cmd, t_main_args *main_args);
int					my_env(t_cmd *cmd, t_main_args *main_args);
int					my_export(t_cmd *cmd, t_main_args *main_args);
int					my_unset(t_cmd *cmd, t_main_args *main_args);
int					my_exit(t_cmd *cmd, t_main_args *main_args, int pid);
// linked_list_pids
t_pids				*ft_lstnew3(int pid);
void				ft_lstadd_back3(t_pids **lst, t_pids *new);
t_pids				*ft_lstlast3(t_pids *lst);

//execute
void				execute(t_cmd *cmd, t_main_args *main_args, int num_cmds);
char				*bring_path(t_main_args *main_args, char *cmd);
void				handle_builtins(t_cmd *cmd, int *built);
char				*my_get_env(char *var, t_main_args *main_args);
void				print_error(char *s);
char				**mini_split(char *var);
int					if_not_file(int i, char *cmd, int *built);

//execute1
int					count(char **param, int c);
int					get_index(char *s, char c);
int					arr_builtins(int i, t_cmd *cmd, t_main_args *main_args,
						int pid);

//execute2
int					here_doc(char **delimeter, int fd[2]);
void				set_input(t_fds *fds, t_io *info_file, t_norm_sake *norm);
void				set_output(t_fds *fds, t_io *info_file);
int					size_cmd(t_cmd *cmd);
//execut3
void				first_child(int *fdpipe, t_fds *fds, t_cmd *cmd,
						t_norm_sake *norm);
void				middle_child(int **fdpipe, t_fds *fds, t_cmd *cmd,
						t_norm_sake *norm);
void				last_child(int *fdpipe, t_fds *fds, t_cmd *cmd,
						t_norm_sake *norm);
void				allocat_pipe(int num_cmds, t_norm_sake *norm);
void				open_files(t_list *order_file, t_fds *fds,
						t_norm_sake *norm);
//execute4
void				free_pipe(int i, int **f);
void				dup2_and_excute(t_fds fds, t_main_args *main_args,
						t_cmd *cmd);
void				make_process(t_main_args *main_args, t_cmd *tmp, t_cmd *cmd,
						t_norm_sake *norm);
int					is_directory(char *cmd);
void				print_exit(char *s, int status);
//execute5
int					init_infile(t_fds fds);
int					init_outfile(t_fds fds);
int					only_one(t_cmd *cmd, t_main_args *main_args, int *built,
						t_norm_sake *norm);
void				wait_process(t_pids *pids);
void				begin(int num_cmds, t_norm_sake *norm, t_cmd *cmd);

//execute6
void				dup2_and(t_fds fds, t_cmd *cmd);
void				and_execute(t_main_args *main_args, t_cmd *cmd);
//builtin_cmds1
char				*ft_remmove_plus(char *param);
void				free_2d(char **s);
int					my_set_env(char *var, char *value, t_main_args *main_args);
int					cd_home(char *home_dir, char *prev_dir,
						t_main_args *main_args);
int					cd_some_where(char *dir, char *prev_dir,
						t_main_args *main_args);

//builtin_cmds2
int					check_flag(char *s);
int					my_echo(t_cmd *cmd, t_main_args *main_args);
int					there_equal(char *s);

//builtin_cmds3
int					check_valid(char *s);
void				turn_list_env(t_main_args *main_args);
void				print_export(t_main_args *main_args);
int					env_exist(t_main_args *main_args, char *var,
						int (ft_func)(char *, char **, char **, t_list *));
//builtin_cmds4
void				my_export_pt2(int *i, int *ret, t_cmd *cmd,
						t_main_args *main_args);
int					unset_valid(char *s);
void				unset_var(t_list **lst, char *var);
//builtin_cmds5
void				ft_help(char **c);
int					change_value(char *var, char **key_value_var,
						char **key_value, t_list *tmp);
int					append_value(char *var, char **key_value_var,
						char **key_value, t_list *tmp);
//minishell.c
char				**ft_envdup(char **env);
void				setup_g_vars(int ac, char **av, char **env,
						t_main_args *args);
void				turn_env_list(char **env, t_list **env_lst);

//conv to cmd
t_cmd				*conv_to_cmd(t_list **lst);
//inits
void				init_help(t_help *h);
char				**init_param(int count);
void				count_until_token(t_help *h, t_list *lst);
//checker
int					check_tokens(t_list *lst);
//value geter
t_list				*get_output_files(int count, t_list *lst);
t_list				*get_input_files(int count, t_list *lst);
char				**get_params(int count, t_list *lst);
//deleters
void				free_arr(t_cmd **arr, void (*del)(void *));
void				free_io(void *ios);
void				free_range_arr(void *rang);
void				del_range(void *range);
//ft_atoi
int					ft_atoi(const char *str);
//parsing1
void				print_io(t_list *lst);
void				print_cmds(t_cmd *lst);
void				handle_line(char *line, t_main_args *args);
void				join_lines(t_list **lst);

#endif
