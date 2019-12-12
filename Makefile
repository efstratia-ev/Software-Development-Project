CC=g++
CFLAGS=-Wall -std=c++11 -O3
project: main.o array.o stack.o list.o JoinArray.o radix.o relation.o relations.o results_list.o sort.o SQL.o SQLInfo.o Priority_Queue.o
	$(CC) $(CFLAGS) -o project main.o array.o stack.o list.o JoinArray.o radix.o relation.o relations.o results_list.o sort.o SQL.o SQLInfo.o Priority_Queue.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
array.o: array.cpp
	$(CC) $(CFLAGS) -c array.cpp
list.o: list.cpp
	$(CC) $(CFLAGS) -c list.cpp
stack.o: stack.cpp
	$(CC) $(CFLAGS) -c stack.cpp
JoinArray.o: JoinArray.cpp
	$(CC) $(CFLAGS) -c JoinArray.cpp
radix.o: radix.cpp
	$(CC) $(CFLAGS) -c radix.cpp
relation.o: relation.cpp
	$(CC) $(CFLAGS) -c relation.cpp
relations.o: relations.cpp
	$(CC) $(CFLAGS) -c relations.cpp
results_list.o: results_list.cpp
	$(CC) $(CFLAGS) -c results_list.cpp
sort.o: sort.cpp
	$(CC) $(CFLAGS) -c sort.cpp
SQLInfo.o: SQLInfo.cpp
	$(CC) $(CFLAGS) -c SQLInfo.cpp
SQL.o: SQL.cpp
	$(CC) $(CFLAGS) -c SQL.cpp
Priority_Queue.o: Priority_Queue.cpp
	$(CC) $(CFLAGS) -c Priority_Queue.cpp
.PHONY: clean

clean: 
	rm -f project *.o
