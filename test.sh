# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.sh                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42sp...>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/18 20:14:28 by maurodri          #+#    #+#              #
#    Updated: 2024/12/11 14:15:31 by maurodri         ###   ########.fr        #
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
    ARGS="6 401 99 200 100"
fi

echo -e "\n::BUILDING::"
make re -C ./philo
#make re -C ./philo_bonus 

echo -e "\n::RUNNING::"
$VALG ./philo/philo $ARGS
#$VALG ./philo_bonus/philo_bonus $ARGS

echo '::END::'
