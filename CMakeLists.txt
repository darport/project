cmake_minimum_required(VERSION 3.10)
project(sudokuProject C)

set(CMAKE_C_STANDARD 90)
add_executable(sudokuProject main.c Game.c Game.h Solver.c Solver.h ILPSolver.c ILPSolver.h mainAux.c mainAux.h Parser.c Parser.h LinkedList.c LinkedList.h Stack.c Stack.h gurobi_c.h detSolver.c detSolver.h)