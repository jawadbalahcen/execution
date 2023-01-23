NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
FILES = minishell.c \
    get_next_line.c \
    string1.c \
    string2.c \
    string3.c \
    linked_list.c \
	linked_list2.c \
    parsing.c \
    split_by_sptial_chars.c \
    group_symbols.c \
    replace_env.c \
    builtin_commands.c \
    utils.c \
    execute.c \
    
OFILES = $(FILES:.c=.o)
OBFILES = $(BFILES:.c=.o)

all : $(NAME) clean

$(NAME) : $(OFILES)
	$(CC) -lreadline $(CFLAGS) $(OFILES) -o $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $^

clean :
	rm -f $(OFILES) $(OBFILES)

fclean : clean
	rm -f $(NAME)

re : fclean all