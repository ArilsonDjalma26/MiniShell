CC = cc
CFLAGS = -Wall -Wextra -Werror -g
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
	utils/ft_strcmp.c \
	utils/ft_strchr.c \
	utils/ft_isalnum.c \
	utils/ft_isdigit.c \
	utils/ft_isalpha.c \
	utils/ft_itoa.c \
	utils/ft_atoi.c \
	parser/parser.c \
	parser/utils.c \
	parser/parsers.c \
	expander/expander.c \
	expander/utils.c \
	parser/freezers.c \
	executor/executor.c \
	executor/exec_cmd.c \
	executor/exec_path.c \
	executor/exec_redir.c \
	executor/utils.c \
	executor/built-in/echo.c \
	executor/built-in/cd.c \
	executor/built-in/pwd.c \
	executor/built-in/env.c \
	executor/built-in/export.c \
	executor/built-in/unset.c \
	executor/built-in/exit.c \
	signals/signals.c \
	signals/signals_handler.c

OBJ = $(SRC:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

$(OBJ): $(HEADER)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re