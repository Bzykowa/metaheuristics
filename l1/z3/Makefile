CC = g++
FLAGS = -std=c++17 --pedantic -Wall -Wextra

all: main

main: path.o terrain.o solver.o main.o
	$(CC) $(FLAGS) -o main path.o terrain.o solver.o main.o

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp

path.o: path.cpp
	$(CC) $(FLAGS) -c path.cpp

terrain.o: terrain.cpp
	$(CC) $(FLAGS) -c terrain.cpp

solver.o: solver.cpp
	$(CC) $(FLAGS) -c solver.cpp

clean:
	rm -f *.o
	rm -f main