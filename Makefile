# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/07 19:48:12 by Helene            #+#    #+#              #
#    Updated: 2023/04/24 02:10:03 by hlesny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
NAME_BONUS = pipex_bonus
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address 

LIBFTDIR = ./mylibft
LIBFT = ./mylibft/libft.a

LIBS = $(LIBFT)

SRCSDIR = ./sources/
INCSDIR = ./includes/
OBJSDIR = ./objs/

INC_FILES = pipex.h get_next_line.h pipex_bonus.h
INCS = $(addprefix $(INCSDIR), $(INC_FILES))

SRCS = 			main.c set_execve_args.c pipe_fork_exec.c \
				mandatory_utils.c utils.c heap_utils.c \
				get_next_line.c get_next_line_utils.c
		
SRCS_BONUS = 	main_bonus.c set_execve_args.c pipe_fork_exec.c \
				here_doc.c utils.c heap_utils.c \
				get_next_line.c get_next_line_utils.c
		
OBJS = $(addprefix $(OBJSDIR), $(SRCS:.c=.o))
OBJS_BONUS = $(addprefix $(OBJSDIR), $(SRCS_BONUS:.c=.o))

export C_INCLUDE_PATH = $(INCSDIR):$(LIBFTDIR)
export LIBRARY_PATH = $(LIBFTDIR)

all : $(LIBS) $(NAME) # !!!!!!! modifier un fichier de la libft n'est pas détecté (nothing to make for all quand retape make)

$(NAME) : $(OBJS)
	$(CC) -o $@ $^ $(LIBFT) 

$(LIBS) :
	make -C $(LIBFTDIR)

bonus : $(NAME_BONUS) $(LIBS) 

$(NAME_BONUS) : $(OBJS_BONUS)
	$(CC) -o $@ $^ $(LIBFT)

$(OBJSDIR)%.o: $(SRCSDIR)%.c $(INCS)
	mkdir -p $(OBJSDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJS) $(OBJS_BONUS)
	$(RM) -r $(OBJSDIR)
	make clean -C $(LIBFTDIR)

fclean : clean
	$(RM) $(NAME) $(NAME_BONUS)
	make fclean -C $(LIBFTDIR)

re : fclean all

.PHONY :
	all clean fclean re