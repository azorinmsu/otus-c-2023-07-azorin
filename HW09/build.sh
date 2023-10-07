gcc -c -Wall -Werror -Wextra logger.c
ar -rc liblogger.a logger.o
gcc -Wall -Wextra -Wpedantic example.c -L. -llogger -o main
