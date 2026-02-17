CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
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
	utils/ft_strjoin.c \
	utils/ft_strlen.c \
	utils/ft_strncmp.c \
	parser/parser.c \
	parser/utils.c \
	parser/freezers.c

OBJ = $(SRC:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.c $(HEADER) lexer/lexer.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re