#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/15 17:53:38 by maurodri          #+#    #+#              #
#    Updated: 2024/07/25 00:56:36 by maurodri         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME := philo
FILES := cutlery.c \
	ft_atoi.c \
	main.c \
	phargs.c \
	philo.c \
	philo_routine.c \
	table.c \
	util.c

OBJ_DIR := ./obj/
MANDATORY_OBJS := $(addprefix $(OBJ_DIR), $(patsubst %.c, %.o, $(FILES)))
BONUS_OBJS := $(addprefix $(OBJ_DIR), $(patsubst %.c, %.o, $(BONUS_FILES)))
DEP_FLAGS := -MP -MD
VPATH := ./
CFLAGS := -g3 -lpthread # -Wall -Wextra -Werror 
CC := cc

INCLUDES := -I./ -I$(LIBFT_DIR)/includes
OBJS := $(MANDATORY_OBJS)
ETAGS_BASE := ./

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ $(INCLUDES) -o $@
	etags $$(find $(ETAGS_BASE) -name '*.[ch]') $$(find $(LIBFT_DIR) -name '*.[ch]') --include '~/glibc/TAGS'

$(OBJS): $(OBJ_DIR)%.o : %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEP_FLAGS) -o $@ -c $< $(INCLUDES) 

$(OBJ_DIR):
	@mkdir -p $@


.PHONY: all clean fclean re bonus

clean:
	rm -fr $(OBJ_DIR) **/*~ *~ **/.#*
	$(MAKE) -C $(LIBFT_DIR) fclean

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEP_FILES)
