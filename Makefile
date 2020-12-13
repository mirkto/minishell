# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngonzo <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/28 22:44:15 by ngonzo            #+#    #+#              #
#    Updated: 2020/10/28 22:44:18 by ngonzo           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
FLAGS = -Wall -Werror -Wextra
SRCS = main.c parser.c
OBJS = $(SRCS:%.c=%.o)
LIBFT = ./libs/libft/libft.a
HDR = ./minishell.h

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	gcc $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	$(MAKE) -C ./libs/libft

$(OBJS): %.o: %.c $(HDR)
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	$(MAKE) clean -C ./libs/libft

fclean: clean
	rm -rf $(NAME)
	$(MAKE) fclean -C ./libs/libft

re: fclean all

.PHONY: all, clean, fclean, re
