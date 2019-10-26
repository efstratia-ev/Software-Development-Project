CC=g++
CFLAGS=-Wall -std=c++11 -O3
project1: main.o hashtable.o stack.o list.o inpoutFunctions.o
	$(CC) $(CFLAGS) -o project1 main.o hashtable.o stack.o list.o inpoutFunctions.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
hashtable.o: hashtable.cpp
	$(CC) $(CFLAGS) -c hashtable.cpp
list.o: list.cpp
	$(CC) $(CFLAGS) -c list.cpp
stack.o: stack.cpp
	$(CC) $(CFLAGS) -c stack.cpp
inpoutFunctions.o: inpoutFunctions.cpp
	$(CC) $(CFLAGS) -c inpoutFunctions.cpp

.PHONY: clean

clean: 
	rm -f project1 main.o hashtable.o Index.o list.o inpoutFunctions.o
