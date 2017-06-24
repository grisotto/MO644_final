SHELL := /bin/bash

CC= g++
LDFLAGS= -pthread
TARGET=genetics_seq genetics_fitness genetics_selection genetics_pop


all:compile 

compile:
	$(CC) $(LDFLAGS) genetics_seq.cpp -o genetics_seq 
	$(CC) $(LDFLAGS) genetics_pop.cpp -o genetics_pop 
	$(CC) $(LDFLAGS) genetics_fitness.cpp -o genetics_fitness  
	$(CC) $(LDFLAGS) genetics_selection.cpp -o genetics_selection 


run: sequencial fitness selection pop 

sequencial: 
	./genetics_seq < in100
	./genetics_seq < in200
	./genetics_seq < in308
	./genetics_seq < in401
	./genetics_seq < in513
	
fitness: 
	./genetics_fitness < in100
	./genetics_fitness < in200
	./genetics_fitness < in308
	./genetics_fitness < in401
	./genetics_fitness < in513


selection:
	./genetics_selection < in100
	./genetics_selection < in200
	./genetics_selection < in308
	./genetics_selection < in401
	./genetics_selection < in513



pop:
	./genetics_pop < in308pop
	./genetics_pop < in401pop
	./genetics_pop < in513pop

clean:
	rm -f *.o $(TARGET) output.dat 
	


