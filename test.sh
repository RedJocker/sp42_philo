# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.sh                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/18 20:14:28 by maurodri          #+#    #+#              #
#    Updated: 2024/07/25 23:44:13 by maurodri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

set -x

if [[ "$1" =~ ^[Ll] ]]; then
    VALG="valgrind --leak-check=full "
elif [[ "$1" =~ ^[Tt] ]]; then
    VALG="valgrind --tool=helgrind "
else
    VALG=" "
fi

if [[ "$2" =~ ^[aA] ]]; then
    ARGS="1 800 200 200"
elif [[ "$2" =~ ^[bB] ]]; then
    ARGS="5 800 200 200 100"
elif [[ "$2" =~ ^[cC] ]]; then
    ARGS="5 800 200 200 7"
elif [[ "$2" =~ ^[dD] ]]; then
    ARGS="4 410 200 200 100"
elif [[ "$2" =~ ^[eE] ]]; then
    ARGS="4 310 200 200 100"
else
    ARGS="3 800 200 200"
fi

make && $VALG ./philo/philo $ARGS
