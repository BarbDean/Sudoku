all: main.o sudoku.o digit.o
	g++ -o sudoku main.o sudoku.o digit.o

main.o: main.cpp
	g++ -c main.cpp

sudoku.o: sudoku.cpp sudoku.h
	g++ -c sudoku.cpp

digit.o: digit.cpp digit.h
	g++ -lmath -c digit.cpp
