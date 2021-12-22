CFLAGS = -Wall
LDFLAGS = -lm -pthread # not really needed for this exercise
CC = gcc -std=c99
ECHO = echo "going to compile for target $@"
PROG = ex3_q1

all: $(PROG) test

ex3_q1: ex3_q1_given.o ex3_q1.o
	gcc -Wall ex3_q1.o ex3_q1_given.o -o ex3_q1 -lm -pthread

ex3_q1.o: ex3_q1.c
	echo ECHO
	gcc -Wall -c ex3_q1.c
	
ex3_q1_given.o: ex3_q1_given.c ex3_q1_given.h
	gcc -c ex3_q1_given.c
	
clean: 
	rm -vf *.o $(PROG)  *.log
	
test: $(PROG)
	@echo going to run test..

	./$(PROG)  > out.log > err.log

