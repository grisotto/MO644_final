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
	./genetics_seq < ./input/in100
	./genetics_seq < ./input/in200
	./genetics_seq < ./input/in308
	./genetics_seq < ./input/in401
	./genetics_seq < ./input/in513
	
fitness: 
	./genetics_fitness < ./input/in100
	./genetics_fitness < ./input/in200
	./genetics_fitness < ./input/in308
	./genetics_fitness < ./input/in401
	./genetics_fitness < ./input/in513


selection:
	./genetics_selection < ./input/in100
	./genetics_selection < ./input/in200
	./genetics_selection < ./input/in308
	./genetics_selection < ./input/in401
	./genetics_selection < ./input/in513



pop:
	./genetics_pop < ./input/in308pop
	./genetics_pop < ./input/in401pop
	./genetics_pop < ./input/in513pop

clean:
	rm -f *.o $(TARGET) output.dat 
	


