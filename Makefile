CC = cc
CFLAGS = -Wall -Wextra -Werror
OOTHER_FLAGS = -lreadline
HEADER = minishell.h
NAME = minishell
SRC = main.c \
	lexer/lexer.c \
	lexer/lexer_quotes.c \
	lexer/lexer_tokens.c \
	lexer/lexer_utils.c \
	utils/ft_split.c \
	utils/ft_substr.c \
	utils/ft_strdup.c \
	utils/ft_strlen.c \
	utils/ft_strncmp.c \
OBJ = $(SRC:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OOTHER_FLAGS) $(OBJ) -o $(NAME)

$(OBJ): $(HEADER)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re