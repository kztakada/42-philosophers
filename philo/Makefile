# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: katakada <katakada@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/16 21:21:00 by katakada          #+#    #+#              #
#    Updated: 2025/04/08 19:09:12 by katakada         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	philo

CC				=	cc
DEBUG_CC		=	cc -D IS_DEBUG=1 -g -fsanitize=thread

CFLAGS			=	-Wall -Wextra -Werror

INCS			=	-I includes/

SRC_PATH		=	src/
SRCS			=	$(SRC_PATH)banker.c \
					$(SRC_PATH)init.c \
					$(SRC_PATH)main.c \
					$(SRC_PATH)parse_argv.c \
					$(SRC_PATH)parse_argv__util.c \
					$(SRC_PATH)put_error.c \
					$(SRC_PATH)thread__monitor.c \
					$(SRC_PATH)thread__philo.c \
					$(SRC_PATH)thread__philo__try_to_eat.c \
					$(SRC_PATH)thread__philo__try_to_eat__has_first_priority.c \
					$(SRC_PATH)thread__philo_loop_actions.c \
					$(SRC_PATH)thread__philo_util.c \
					$(SRC_PATH)thread__util.c \
					$(SRC_PATH)tidy_up.c \
					$(SRC_PATH)time__util.c \
					$(SRC_PATH)util.c 

OBJS_PATH		=	objs/
OBJS			=	$(SRCS:src/%.c=objs/%.o)

ISDEBUG = 0

ifeq ($(ISDEBUG), 1)
	CC = $(DEBUG_CC)
endif



$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCS) $(OBJS) -o $(NAME)

$(OBJS_PATH)%.o : $(SRC_PATH)%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@


all:	$(NAME)

debug: 
	$(MAKE) ISDEBUG=1 all


clean:
	rm -rf $(OBJS_PATH)

fclean:
	rm -rf $(OBJS_PATH)
	rm -f $(NAME)

re:		fclean all

.PHONY:	all clean fclean re debug
