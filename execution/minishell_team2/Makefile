NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror 
RLFLAGS = -lreadline
# INCLUDES = -I ~/.brew/opt/readline/include -I ./minishell -fsanitize=address -g3
FILES = minishell.c \
	get_next_line.c \
	string1.c \
	string2.c \
	string3.c \
	string4.c \
	string5.c \
	linked_list.c \
	linked_list2.c \
	linked_list3.c \
	parsing.c \
	qoutes.c \
	replace_env.c \
	execute.c \
	builtin_commands.c \
	conv_to_cmd.c \

OFILES = $(FILES:.c=.o)
OBFILES = $(BFILES:.c=.o)

all : $(NAME) clean

$(NAME) : $(OFILES)
	$(CC) $(CFLAGS) $(RLFLAGS) $(OFILES) $(INCLUDES) -o $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDES)

clean :
	rm -f $(OFILES) $(OBFILES)

fclean : clean
	rm -f $(NAME)

re : fclean all
