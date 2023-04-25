# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asadanow <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/08 15:39:12 by asadanow          #+#    #+#              #
#    Updated: 2023/04/25 18:16:29 by asadanow         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = ./src/main.c \
		./src/env_utils.c \
		./src/signal.c \
		./src/utils.c \
		./src/free_utils.c \
		./src/parsing/ft_split_quotes.c \
		./src/parsing/parsing_main.c \
		./src/parsing/parsing_utils.c \
		./src/parsing/env_variables.c \
		./src/parsing/list_utils.c \
		./src/parsing/create_cmdlst.c \
		./src/parsing/heredoc.c \
		./src/parsing/cmdlst_utils.c \
		./src/get_next_line/get_next_line.c \
		./src/get_next_line/get_next_line_utils.c \
		./src/exec/exec_main.c \
		./src/exec/processes.c \
		./src/exec/pipes_wait.c \
		./src/exec/utils1.c \
		./src/exec/utils2.c \
		./src/exec/split_pipex.c \
		./src/builtins/cd.c \
		./src/builtins/echo.c \
		./src/builtins/env.c \
		./src/builtins/exit.c \
		./src/builtins/export.c \
		./src/builtins/pwd.c \
		./src/builtins/unset.c

LIBDIR = ./libft/

CC = gcc -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	cd $(LIBDIR) && $(MAKE)
	$(CC) $(OBJ) -lreadline ./libft/libft.a -o $(NAME)

.c.o:
	$(CC) -c $< -I ./includes -I ./libft -o $@

clean:
	/bin/rm -f $(OBJ)
	cd $(LIBDIR) && $(MAKE) clean

fclean: clean
	/bin/rm -f $(NAME)
	cd $(LIBDIR) && $(MAKE) fclean

re: fclean all
