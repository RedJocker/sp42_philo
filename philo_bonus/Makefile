#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/15 17:53:38 by maurodri          #+#    #+#              #
#    Updated: 2024/12/11 13:07:04 by maurodri         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

BONUS_DIR := ./

BONUS_FILES := main_bonus.c \
		ft_atoi_bonus.c \
		ft_itoa_bonus.c \
		philo_routine_bonus.c \
		philo_monitor_bonus.c \
		philo_bonus.c \
		philo_logger.c \
		util_bonus.c \
		phargs_bonus.c \
		table_bonus.c 

OBJ_DIR := ./obj/
BONUS_OBJS := $(addprefix $(OBJ_DIR), $(patsubst %.c, %.o, $(BONUS_FILES)))
DEP_FLAGS := -MP -MD
VPATH := $(BONUS_DIR)
CFLAGS := -g3 -lpthread -Wall -Wextra -Werror
CC := cc

NAME := $(BONUS_DIR)philo_bonus
INCLUDES := -I$(BONUS_DIR)
OBJS := $(BONUS_OBJS)
ETAGS_BASE := $(BONUS_DIR)

DEP_FILES := $(patsubst %.o, %.d, $(OBJS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ $(INCLUDES) -o $@
	etags $$(find $(ETAGS_BASE) -name '*.[ch]') --include '~/glibc/TAGS'

$(OBJS): $(OBJ_DIR)%.o : %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEP_FLAGS) -o $@ -c $< $(INCLUDES) 

$(OBJ_DIR):
	@mkdir -p $@

.PHONY: all clean fclean re

clean:
	rm -fr $(OBJ_DIR) **/*~ *~ **/.#*

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEP_FILES)
