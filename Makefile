CC = clang

TARGET = main

CFLAGS = -std=c99 -O0 -g -Wall -Wpedantic -Wextra -Werror -lpthread

all:
	$(CC) $(CFLAGS) \
	./ThreadPool.c \
	./main.c \
	-o $(TARGET)

clean:
	rm $(TARGET)