# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Helene <Helene@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/07 19:48:12 by Helene            #+#    #+#              #
#    Updated: 2023/04/09 21:33:34 by Helene           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = gcc
RM = rm -f
CFLAGS = -g3 #-fsanitize=address 

LIBFTDIR = ./mylibft
LIBFT = ./mylibft/libft.a

LIBS = $(LIBFT)

SRCSDIR = ./sources/
INCSDIR = ./includes/
OBJSDIR = ./objs/

INC_FILES = pipex.h get_next_line.h
INCS = $(addprefix $(INCSDIR), $(INC_FILES))

SRCS = 	pipex2.c \
		get_next_line.c get_next_line_utils.c
		
OBJS = $(addprefix $(OBJSDIR), $(SRCS:.c=.o))

export C_INCLUDE_PATH = $(INCSDIR):$(LIBFTDIR)
export LIBRARY_PATH = $(LIBFTDIR)

all : $(LIBS) $(NAME)

$(NAME) : $(OBJS)
	$(CC) -o $@ $^ $(LIBFT)

$(OBJSDIR)%.o: $(SRCSDIR)%.c $(INCS)
	mkdir -p $(OBJSDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBS) :
	make -C $(LIBFTDIR)

# pourquoi $(RM) $(OBJS) -r $(OBJSDIR) (ie les deux sur la meme ligne) ne marche pas ? 
clean :
	$(RM) $(OBJS)
	$(RM) -r $(OBJSDIR)
	make clean -C $(LIBFTDIR)

fclean : clean
	$(RM) $(NAME)
	make fclean -C $(LIBFTDIR)

re : fclean all

.PHONY :
	all clean fclean re