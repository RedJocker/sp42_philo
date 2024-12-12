# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.sh                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/18 20:14:28 by maurodri          #+#    #+#              #
#    Updated: 2024/12/12 11:41:38 by maurodri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

set -x
set -e

echo "::ARGUMENTS::"
echo -e "\n::$1 = first arg:(l=leak-check|t=tool-helgrind|*='')::"
if [[ "$1" =~ ^[Ll] ]]; then    
    VALG="valgrind --leak-check=full "
elif [[ "$1" =~ ^[Tt] ]]; then
    VALG="valgrind --tool=helgrind "
else
    VALG=" "
fi

echo -e "\n::$2 = second arg:(a=caseA|b=caseB|c=caseC|...|i=caseI|*=caseDefault)::"
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
elif [[ "$2" =~ ^[fF] ]]; then
    ARGS="4 100 100 1 100"
elif [[ "$2" =~ ^[gG] ]]; then
    ARGS="4 100 1 100 100"
elif [[ "$2" =~ ^[hH] ]]; then
    ARGS="4 100 10000 1 100"
elif [[ "$2" =~ ^[iI] ]]; then
    ARGS="4 100 1 10000 100"
else
    #ARGS="3 800 200 200"
    #ARGS="5 820 200 200 100"
    #ARGS="2 401 99 200 100"
    #ARGS="3 401 99 200 100"
    #ARGS="4 401 99 200 100"
    #ARGS="5 401 99 200 100"
    #ARGS="6 401 99 200 100"
    #ARGS="100 415 200 200 100"
    ARGS="2 403 200 200 100"
fi

echo -e "\n::BUILDING::"
if [[ "$3" =~ ^[bB] ]]; then
    make re -C ./philo_bonus 
else
    make re -C ./philo
fi

echo -e "\n::RUNNING::"
if [[ "$3" =~ ^[bB] ]]; then
    $VALG ./philo_bonus/philo_bonus $ARGS
else
    $VALG ./philo/philo $ARGS
fi
    
echo '::END::'
