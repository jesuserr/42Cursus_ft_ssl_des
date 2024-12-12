# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/19 17:09:51 by jesuserr          #+#    #+#              #
#    Updated: 2024/12/09 11:10:33 by jesuserr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT_DIR = libft/

NAME = ft_ssl
SRCS = main.c parser.c md5.c print_utils.c sha256.c bitwise_utils.c sha224.c \
	   sha512.c sha384.c interactive.c
PATH_SRCS = ./srcs/
PATH_INCS = ./srcs/incs/
PATH_OBJS = ./objs/
PATH_DEPS = ./objs/

OBJS = $(addprefix $(PATH_OBJS), $(SRCS:.c=.o))
DEPS = $(addprefix $(PATH_DEPS), $(SRCS:.c=.d))

INCLUDE = -I./ -I./libft/includes/
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g -pedantic -Wshadow
LDFLAGS = -lm

NORM = $(addprefix $(PATH_SRCS), $(SRCS)) $(PATH_INCS)*.h
GREEN = "\033[0;92m"
RED = "\033[0;91m"
BLUE = "\033[0;94m"
NC = "\033[37m"

all: makelibft $(NAME)

makelibft:
	@make --no-print-directory -C $(LIBFT_DIR)	
	@echo ${GREEN}"Libft Compiled!\n"${NC};

$(PATH_OBJS)%.o: $(PATH_SRCS)%.c Makefile
	@mkdir -p $(PATH_OBJS)
	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT_DIR)libft.a
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_DIR)libft.a -o $@ $(LDFLAGS)
	@echo ${GREEN}"ft_ssl Compiled!\n"${NC};
-include $(DEPS)

clean:
	@make clean --no-print-directory -C $(LIBFT_DIR)	
	@rm -rf $(PATH_OBJS)	
		
fclean:
	@make fclean --no-print-directory -C $(LIBFT_DIR)
	@rm -rf $(PATH_OBJS)
	$(RM) $(NAME)

norm:
	@echo ${BLUE}"\nChecking Norminette..."${NC}
	@if norminette $(NORM); then echo ${GREEN}"Norminette OK!\n"${NC}; \
	else echo ${RED}"Norminette KO!\n"${NC}; \
	fi

re: fclean all

.PHONY: all clean fclean re makelibft norm