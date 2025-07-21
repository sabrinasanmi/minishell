NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra -g3

INCS = include libft # Colocar as pastas onde tem .h
CPPFLAGS = $(addprefix -I,$(INCS)) -MMD -MP # Facilitar o include das pastas
SRC = $(shell find . -name "*.c" ! -path "./libft/*")

OBJS = $(SRC:%.c=%.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

# Compila libft primeiro
$(LIBFT):
	make -C $(LIBFT_DIR)

	
# Compila o minishell
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline
	chmod +x $(NAME)
	rm -f $(OBJS) $(OBJS:.o=.d)
	echo	"███    ███ ██ ███    ██ ██ ███████ ██   ██ ███████ ██      ██     "
	echo	"████  ████ ██ ████   ██ ██ ██      ██   ██ ██      ██      ██     "
	echo	"██ ████ ██ ██ ██ ██  ██ ██ ███████ ███████ █████   ██      ██     "
	echo	"██  ██  ██ ██ ██  ██ ██ ██      ██ ██   ██ ██      ██      ██     "
	echo	"██      ██ ██ ██   ████ ██ ███████ ██   ██ ███████ ███████ ███████"

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 

.SILENT: