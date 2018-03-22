
CC = g++
FLAGS = -g -Wall

all: main.cpp
	$(CC) $(FLAGS) $^ -o easyargs