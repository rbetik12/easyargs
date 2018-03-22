
CC = g++
FLAGS = -g -Wall

all: main.
	$(CC) $(FLAGS) $^ -o easyargs

tests:


# use make lib to create easyargs.so, which you can then link with -ldl
lib:
