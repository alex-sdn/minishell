# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asadanow <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 18:38:29 by asadanow          #+#    #+#              #
#    Updated: 2022/11/11 15:29:07 by asadanow         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = ft_isalpha.c \
	ft_isdigit.c \
	ft_isalnum.c \
	ft_isascii.c \
	ft_isprint.c \
	ft_strlen.c \
	ft_tolower.c \
	ft_toupper.c \
	ft_memset.c \
	ft_bzero.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_strncmp.c \
	ft_strnstr.c \
	ft_atoi.c \
	ft_strdup.c \
	ft_calloc.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_strtrim.c \
	ft_split.c \
	ft_itoa.c \
	ft_strmapi.c \
	ft_striteri.c \
	ft_putchar_fd.c \
	ft_putstr_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_lstnew.c \
    ft_lstadd_front.c \
    ft_lstadd_back.c \
    ft_lstsize.c \
    ft_lstlast.c \
    ft_lstdelone.c \
    ft_lstclear.c \
    ft_lstiter.c \
    ft_lstmap.c

CC = gcc -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME):$(OBJ)
	ar rc $(NAME) $(OBJ)

.c.o:
	$(CC) -c $< -I ./ -o $@

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
