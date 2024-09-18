NAME = pipex

SRC = 	pipex.c \
		libft.c  ft_split.c \
		errors.c \
		paths.c \
		get_next_line/get_next_line.c get_next_line/get_next_line_utils.c \
		here_doc.c \

OBJ = $(SRC:.c=.o)

CC = cc
# CFLAGS = -Wall -Wextra -Werror
REM		=	rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(REM) $(OBJ)

fclean: 
	$(REM) $(OBJ)
	$(REM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus