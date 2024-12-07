#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/15 17:53:38 by maurodri          #+#    #+#              #
#    Updated: 2024/12/07 03:49:17 by maurodri         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

MANDATORY_DIR := ./philo
BONUS_DIR := ./philo_bonus

FILES := cutlery.c \
	ft_atoi.c \
	main.c \
	phargs.c \
	philo.c \
	philo_routine.c \
	table.c \
	util.c

BONUS_FILES := main_bonus.c \
		ft_atoi_bonus.c \
		ft_itoa_bonus.c \
		philo_routine_bonus.c \
		philo_monitor_bonus.c \
		philo_bonus.c \
		util_bonus.c \
		phargs_bonus.c \
		table_bonus.c 

OBJ_DIR := ./obj/
MANDATORY_OBJS := $(addprefix $(OBJ_DIR), $(patsubst %.c, %.o, $(FILES)))
BONUS_OBJS := $(addprefix $(OBJ_DIR), $(patsubst %.c, %.o, $(BONUS_FILES)))
DEP_FLAGS := -MP -MD
VPATH := ./ $(MANDATORY_DIR) $(BONUS_DIR)
CFLAGS := -g3 -lpthread # -Wall -Wextra -Werror 
CC := cc


ifdef WITH_BONUS
	NAME := $(BONUS_DIR)/philo_bonus
	INCLUDES := -I./$(BONUS_DIR)
	OBJS := $(BONUS_OBJS)
	ETAGS_BASE := $(BONUS_DIR)
else
	NAME := $(MANDATORY_DIR)/philo
	INCLUDES := -I./$(MANDATORY_DIR)
	OBJS := $(MANDATORY_OBJS)
	ETAGS_BASE := $(MANDATORY_DIR)
endif

DEP_FILES := $(patsubst %.o, %.d, $(OBJS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ $(INCLUDES) -o $@
	etags $$(find $(ETAGS_BASE) -name '*.[ch]') --include '~/glibc/TAGS'

$(OBJS): $(OBJ_DIR)%.o : %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEP_FLAGS) -o $@ -c $< $(INCLUDES) 

$(OBJ_DIR):
	@mkdir -p $@

bonus:
	$(MAKE) WITH_BONUS=1

.PHONY: all clean fclean re bonus

clean:
	rm -fr $(OBJ_DIR) **/*~ *~ **/.#*

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEP_FILES)
