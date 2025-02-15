NAME = minishell
FLAGS = -g -Wall -Werror -Wextra #-fsanitize=address
SRCS = main.c builtins.c ft_execve.c utils.c parser.c lexer.c blt_export.c\
		blt_export_utils.c blt_env.c blt_cd_and_pwd.c signal.c ft_redirect.c\
		ft_pipe.c parser_utils.c parser_quote_remover.c exec_loop.c inits.c
OBJS = $(SRCS:%.c=%.o)
LIBFT = ./libft/libft.a
HDR = ./minishell.h

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	gcc $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	$(MAKE) -C ./libft

$(OBJS): %.o: %.c $(HDR)
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	$(MAKE) clean -C ./libft

fclean: clean
	rm -rf $(NAME)
	$(MAKE) fclean -C ./libft

re: fclean all

.PHONY: all, clean, fclean, re
