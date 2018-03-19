
CC = g++
FLAGS = -g -Wall

all: src/easyargs.cpp
	$(CC) $(FLAGS) $^ -o easyargs