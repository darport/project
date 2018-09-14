CC = gcc
OBJS = main.o Solver.o  ILPSolver.o Game.o Parser.o mainAux.o Stack.o LinkedList.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS)  $(GUROBI_LIB) -o $@ 
main.o: Game.h Parser.h mainAux.h  
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Game.o: Game.h Solver.h ILPSolver.h MainAux.h LinkedList.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Parser.o: Game.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Solver.o:  Game.h Stack.h mainAux.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
ILPSolver.o:  Game.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
MainAux.o: Game.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Stack.o: Stack.h mainAux.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
LinkedList.o: LinkedList.h Game.h mainAux.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c

clean:
	rm -rf *.o $(EXEC) $(OBJS)