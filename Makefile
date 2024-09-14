NAME = pipex

SRC = 	pipex.c \
		libft.c  ft_split.c \

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