CC = g++
FLAGS = -g -Wall

tests: main.cpp easyargs.o
	$(CC) $(FLAGS) -o $@ $?
	./tests

easyargs.o: easyargs.cpp
	$(CC) $(FLAGS) -c -fPIC $? -o $@

lib: libezrg.so

libezrg.so: easyargs.o
	$(CC) -shared $? -o $@

clean:
	rm *.o
	rm tests

