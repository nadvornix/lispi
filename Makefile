
CC=g++
CFLAGS=-Wall -pedantic -ansi -O0 -ggdb

all: datetypes.o enviroment.o main.o parser.o stdlibc.o tokenizer.o repl.o exceptions.o
	$(CC) $(CFLAGS) -o lisp datetypes.o enviroment.o main.o parser.o stdlibc.o tokenizer.o repl.o exceptions.o

clean:
	rm *.o; rm lisp; rm *plist

compile: all
	

run:
	./lisp test.lisp

main.o: main.cpp enviroment.h datetypes.h parser.h tokendefs.h tokenizer.h 
	$(CC) $(CFLAGS) -c main.cpp

parser.o: parser.cpp tokendefs.h datetypes.h enviroment.h tokenizer.h parser.h
	$(CC) $(CFLAGS) -c parser.cpp

enviroment.o: enviroment.cpp datetypes.h enviroment.h stdlibc.h
	$(CC) $(CFLAGS) -c enviroment.cpp

datetypes.o: datetypes.cpp datetypes.h enviroment.h
	$(CC) $(CFLAGS) -c datetypes.cpp

tokenizer.o: tokenizer.cpp tokendefs.h datetypes.h enviroment.h tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.cpp

stdlibc.o: stdlibc.h datetypes.h enviroment.h stdlibc.cpp
	$(CC) $(CFLAGS) -c stdlibc.cpp

repl.o: repl.h repl.cpp
	$(CC) $(CFLAGS) -c repl.cpp

exceptions.o: exceptions.h exceptions.cpp
	$(CC) $(CFLAGS) -c exceptions.cpp