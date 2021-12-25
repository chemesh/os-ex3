wCFLAGS = -Wall -c
LDFLAGS = -lm -pthread # not really needed for this exercise
CC = gcc -std=c99
ECHO = echo "going to compile for target $@"
PROG = ex3_q1

all: $(PROG) test

ex3_q1: ex3_q1.o utils.o ex3_q1_given.o
	gcc -Wall ex3_q1.o ex3_q1_given.o utils.o -o ex3_q1 $(LDFLAGS)

ex3_q1.o: ex3_q1.c ex3_q1.h ex3_q1_given.o
	$(ECHO)
	gcc $(wCFLAGS) ex3_q1.c -lm
	
ex3_q1_given.o: ex3_q1_given.c ex3_q1_given.h
	$(ECHO)
	gcc $(wCFLAGS) ex3_q1_given.c $(LDFLAGS)

utils.o: utils.c ex3_q1.h
	$(ECHO)
	gcc $(wCFLAGS) utils.c -lm
	
clean: 
	rm -vf *.o $(PROG) *.log
	
test: $(PROG)
	@echo going to run test..

	./$(PROG)  > out.log 2> err.log

