CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g

FILES_SRCS = philo.c actions.c monitoring.c prints.c utils.c parsing.c safe_functions.c safe_pthread.c
FILES_OBJS = $(FILES_SRCS:.c=.o)

EXECUTABLE = philo
HEADER = philo.h

all: $(EXECUTABLE)

$(EXECUTABLE): $(FILES_OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(FILES_OBJS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(FILES_OBJS)

fclean: clean
	rm -f $(EXECUTABLE)

re: fclean all
